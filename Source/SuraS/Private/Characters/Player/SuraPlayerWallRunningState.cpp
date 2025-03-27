// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/SuraPlayerWallRunningState.h"

#include "ActorComponents/ACPlayerMovmentData.h"
#include "Camera/CameraComponent.h"
#include "Characters/Player/SuraCharacterPlayer.h"
#include "Characters/Player/SuraPlayerFallingState.h"
#include "Characters/Player/SuraPlayerJumpingState.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


USuraPlayerWallRunningState::USuraPlayerWallRunningState()
{
	StateDisplayName = "Wall Running";
	StateType = EPlayerState::WallRunning;
}


void USuraPlayerWallRunningState::EnterState(ASuraCharacterPlayer* Player)
{
	Super::EnterState(Player);

	WallRunEnterZVelocityThreshold = Player->GetPlayerMovementData()->GetWallRunEnterZVelocityThreshold();
	WallRunEnterVerticalForce = Player->GetPlayerMovementData()->GetWallRunEnterVerticalForce();
	WallRunCameraTiltAngle = Player->GetPlayerMovementData()->GetWallRunCameraTiltAngle();

	PlayerController = Player->GetController<APlayerController>();

	MaxDuration = Player->GetPlayerMovementData()->GetWallRunningMaxDuration();
	
	
	ElapsedTime = 0.f;
	bShouldRotateCamera = false;
	bFrontWallFound = false;
	Player->GetCharacterMovement()->GravityScale = 0.f;
	Player->GetCharacterMovement()->AirControl = 1.f;
	Player->JumpsLeft = Player->MaxJumps;
	WallRunSide = Player->WallRunSide;
	
	float PlayerVelocityZ = Player->GetCharacterMovement()->Velocity.Z;
	if (PlayerVelocityZ > 0.f)
	{
		if (PlayerVelocityZ < WallRunEnterZVelocityThreshold)
		{
			bShouldSlideUp = true;
		}
	}
	else
	{
		if (PlayerVelocityZ < -WallRunEnterZVelocityThreshold)
		{
			bShouldSlideDown = true;
		}
	}
	
	// Need to make variable for minimum wall run velocity!! Currently set to WalkSpeed for testing purposes.
	StateEnterVelocity = FMath::Clamp(Player->GetCharacterMovement()->Velocity.Size(), Player->GetPlayerMovementData()->GetRunSpeed()
		, Player->GetPlayerMovementData()->GetMaxWallRunSpeed());
}


void USuraPlayerWallRunningState::TiltController(ASuraCharacterPlayer* Player, float DeltaTime)
{

	float Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(Player->GetActorForwardVector(), Player->WallRunDirection)));
	float Alpha = FMath::Abs(Angle / 180.f);
	float TargetRoll = 0.f;
	
	if (WallRunSide == EWallSide::Left)
	{
		TargetRoll = FMath::Lerp(WallRunCameraTiltAngle, -WallRunCameraTiltAngle, Alpha);
	}
	else if (WallRunSide == EWallSide::Right)
	{
		TargetRoll = FMath::Lerp(-WallRunCameraTiltAngle, WallRunCameraTiltAngle, Alpha);
	}

	FRotator CurrentControlRotation = Player->GetControlRotation();
	FRotator NewRotation = FMath::RInterpTo(CurrentControlRotation, FRotator(CurrentControlRotation.Pitch, CurrentControlRotation.Yaw, TargetRoll), DeltaTime, 10.f);

	Player->GetController()->SetControlRotation(NewRotation);
}

void USuraPlayerWallRunningState::UpdateState(ASuraCharacterPlayer* Player, float DeltaTime)
{
	Super::UpdateState(Player, DeltaTime);

	Player->StartCamShake(Player->WallRunCamShake);

	if (Player->bCrouchTriggered)
	{
		Player->ChangeState(Player->FallingState);
		return;
	}

	if (ElapsedTime < MaxDuration)
	{
		ElapsedTime += DeltaTime;
	}
	else
	{
		Player->ChangeState(Player->FallingState);
		return;
	}

	if (ElapsedTime < 0.)

	Player->InterpCapsuleHeight(1.f, DeltaTime);

	SetPlayerWallOffsetLocation(Player, DeltaTime);

	if (WallRunSide == EWallSide::Left && Player->RightAxisInputValue >= 0.f ||
		WallRunSide == EWallSide::Right && Player->RightAxisInputValue <= 0.f)
	{
		if (Player->IsFallingDown())
		{
			Player->ChangeState(Player->FallingState);
			return;
		}
		Player->ChangeState(Player->JumpingState);
		return;
	}

	TiltController(Player, DeltaTime);

	
	if (Player->bJumpTriggered && Player->JumpsLeft > 0)
	{
		Player->JumpsLeft--;
		FVector LaunchDirectionXY = FVector::ZeroVector;
		if (Player->WallRunSide == EWallSide::Left)
		{
			LaunchDirectionXY = FVector::CrossProduct(Player->WallRunDirection, FVector::DownVector).GetSafeNormal();
		}
		else if (Player->WallRunSide == EWallSide::Right)
		{
			LaunchDirectionXY = FVector::CrossProduct(Player->WallRunDirection, FVector::UpVector).GetSafeNormal();
		}
		FVector LaunchVector = LaunchDirectionXY * 500.f +
			FVector::UpVector * Player->GetPlayerMovementData()->GetPrimaryJumpZSpeed();
		Player->LaunchCharacter(LaunchVector, false, true);
		Player->ChangeState(Player->JumpingState);
		return;
	}

	float Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(Player->WallRunDirection, Player->GetActorForwardVector())));
	if (Angle > 120.f)
	{
		Player->ChangeState(Player->FallingState);
		return;
	}

	
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Player);

	if (!bFrontWallFound)
	{
		bool bFrontWallHit = GetWorld()->SweepSingleByChannel(
		FrontWallHit,
		Player->GetActorLocation(),
		Player->GetActorLocation() + Player->WallRunDirection * 55.f,
		FQuat::Identity,
		ECC_GameTraceChannel2,
		FCollisionShape::MakeCapsule(34.f, Player->GetDefaultCapsuleHalfHeight()),
		Params);

		if (bFrontWallHit)
		{
			if (FrontWallHit.ImpactNormal.Z > -0.05f && FrontWallHit.ImpactNormal.Z < Player->GetCharacterMovement()->GetWalkableFloorZ())
			{
				bFrontWallFound = true;
			}
		}
	}

	if (bFrontWallFound && FrontWallHit.IsValidBlockingHit())
	{
		
		if (WallRunSide == EWallSide::Left)
		{
			Player->WallRunDirection = FVector::CrossProduct(FrontWallHit.ImpactNormal, FVector::UpVector).GetSafeNormal();
			float ForwardAndWallAngle = FMath::RadiansToDegrees(
				FMath::Acos(FVector::DotProduct(Player->WallRunDirection, Player->GetActorForwardVector())));
			
			if (FVector::CrossProduct(Player->WallRunDirection, Player->GetActorForwardVector()).Z < 0.f &&
				ForwardAndWallAngle > 60.f)
			{
				Player->ChangeState(Player->FallingState);
				return;
			}
		}
		else if (WallRunSide == EWallSide::Right)
		{
			Player->WallRunDirection = FVector::CrossProduct(FrontWallHit.ImpactNormal, FVector::DownVector).GetSafeNormal();
			float ForwardAndWallAngle = FMath::RadiansToDegrees(
				FMath::Acos(FVector::DotProduct(Player->WallRunDirection, Player->GetActorForwardVector())));
			if (FVector::CrossProduct(Player->WallRunDirection, Player->GetActorForwardVector()).Z > 0.f &&
				ForwardAndWallAngle > 60.f)
			{
				Player->ChangeState(Player->FallingState);
				return;
			}
		}
		bFrontWallFound = false;
	}
	
	FVector LineTraceEnd = FVector::ZeroVector;
	if (Player->WallRunSide == EWallSide::Left)
	{
		LineTraceEnd = Player->GetActorLocation() +
			FVector::CrossProduct(Player->WallRunDirection, FVector::UpVector).GetSafeNormal() * 200.f;
	}
	else if (Player->WallRunSide == EWallSide::Right)
	{
		LineTraceEnd = Player->GetActorLocation() +
			FVector::CrossProduct(Player->WallRunDirection, FVector::DownVector).GetSafeNormal() * 200.f;
	}

	bool bWallHit = GetWorld()->LineTraceSingleByChannel(
		WallHit,
		Player->GetActorLocation(),
		LineTraceEnd,
		ECC_GameTraceChannel2,
		Params);

	if (bWallHit && WallHit.IsValidBlockingHit())
	{
		FVector WallNormalXY = FVector(WallHit.ImpactNormal.X, WallHit.ImpactNormal.Y, 0.f);
		
		FVector NewWallRunDirection = FVector::ZeroVector;
		if (Player->WallRunSide == EWallSide::Left)
		{
			NewWallRunDirection = FVector::CrossProduct(WallNormalXY, FVector::UpVector).GetSafeNormal();
		}
		else if (Player->WallRunSide == EWallSide::Right)
		{
			NewWallRunDirection = FVector::CrossProduct(WallNormalXY, FVector::DownVector).GetSafeNormal();
		}
		Player->WallRunDirection = NewWallRunDirection;
		FVector NewVelocity = FVector(NewWallRunDirection.X, NewWallRunDirection.Y, 0.f) * StateEnterVelocity;
		FVector ZDeltaVelocity = FVector::ZeroVector;
		if (ElapsedTime < 0.75f)
		{
			float Alpha = ElapsedTime / 0.75f;
			if (bShouldSlideUp)
			{
				ZDeltaVelocity = FVector::UpVector * FMath::Lerp(0, WallRunEnterVerticalForce, 1 - Alpha * Alpha);
			}
			else if (bShouldSlideDown)
			{
				ZDeltaVelocity = FVector::DownVector * FMath::Lerp(0, WallRunEnterVerticalForce, 1 - Alpha * Alpha);
			}
		}
		NewVelocity += ZDeltaVelocity;
		Player->GetCharacterMovement()->Velocity = NewVelocity;
	}
	else
	{
		Player->ChangeState(Player->FallingState);
		return;
	}

}

void USuraPlayerWallRunningState::ExitState(ASuraCharacterPlayer* Player)
{
	Super::ExitState(Player);
	Player->GetCharacterMovement()->GravityScale = Player->DefaultGravityScale;
	Player->GetCharacterMovement()->AirControl = Player->GetPlayerMovementData()->GetAirControl();
	Player->WallRunSide = EWallSide::None;
	ElapsedTime = 0.f;
	bFrontWallFound = false;
	bShouldRotateCamera = false;
	Player->bShouldRestoreCameraTilt = true;
	bShouldSlideUp = false;
	bShouldSlideDown = false;
}

void USuraPlayerWallRunningState::Look(ASuraCharacterPlayer* Player, const FVector2D& InputVector)
{
	Super::Look(Player, InputVector);
	Player->AddControllerYawInput(InputVector.X);
	Player->AddControllerPitchInput(InputVector.Y);
}

void USuraPlayerWallRunningState::Move(ASuraCharacterPlayer* Player, const FVector2D& InputVector)
{
	Super::Move(Player, InputVector);
}

void USuraPlayerWallRunningState::SetPlayerWallOffsetLocation(ASuraCharacterPlayer* Player, float DeltaTime)
{
	if (WallHit.ImpactNormal != FVector::ZeroVector)
	{
		FVector NewLocation = FMath::VInterpTo(Player->GetActorLocation(),
											   Player->GetActorLocation() + WallHit.ImpactNormal * 30.f, DeltaTime, 15.f);
	}
}