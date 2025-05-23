// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/SuraPlayerJumpingState.h"

#include "ActorComponents/ACPlayerMovmentData.h"
#include "Camera/CameraComponent.h"
#include "Characters/Player/SuraCharacterPlayer.h"
#include "Characters/Player/SuraPlayerDashingState.h"
#include "Characters/Player/SuraPlayerFallingState.h"
#include "Characters/Player/SuraPlayerHangingState.h"
#include "Characters/Player/SuraPlayerMantlingState.h"
#include "Characters/Player/SuraPlayerRunningState.h"
#include "Characters/Player/SuraPlayerWalkingState.h"
#include "Characters/Player/SuraPlayerWallRunningState.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

USuraPlayerJumpingState::USuraPlayerJumpingState()
{
	StateDisplayName = "Jumping";
	StateType = EPlayerState::Jumping;
}


void USuraPlayerJumpingState::EnterState(ASuraCharacterPlayer* Player)
{
	Super::EnterState(Player);

	PlayerController = Player->GetController<APlayerController>();

	if (Player->JumpsLeft == 1)
	{
		Player->StartCamShake(Player->PrimaryJumpCamShake);
	}
	else if (Player->JumpsLeft == 0)
	{
		Player->StartCamShake(Player->DoubleJumpCamShake);
	}

	ElapsedTimeFromHanging = 0.f;

	if (Player->GetPreviousState()->GetStateType() == EPlayerState::Hanging)
	{
		bShouldCheckLedge = false;
	}
	else
	{
		bShouldCheckLedge = true;
	}
	

	if (Player->GetPreviousGroundedState()->GetStateType() == EPlayerState::Dashing)
	{
		bShouldUpdateSpeed = true;
		SpeedTransitionTime = Player->GetPlayerMovementData()->GetRunDashTransitionDuration();
		float DashEndSpeed = Player->GetPlayerMovementData()->GetDashEndSpeed();
		float RunSpeed = Player->GetPlayerMovementData()->GetRunSpeed();
		SpeedChangePerSecond = (RunSpeed - DashEndSpeed) / SpeedTransitionTime;
	}
	ElapsedTimeFromWallRun = 0.f;
}

void USuraPlayerJumpingState::UpdateBaseMovementSpeed(ASuraCharacterPlayer* Player, float DeltaTime)
{
	if (bShouldUpdateSpeed)
	{
		float CurrentBaseSpeed = Player->GetBaseMovementSpeed();
		float TargetRunSpeed = Player->GetPlayerMovementData()->GetRunSpeed();
		if (SpeedChangePerSecond > 0 && CurrentBaseSpeed < TargetRunSpeed ||
			SpeedChangePerSecond < 0 && CurrentBaseSpeed > TargetRunSpeed)
		{
			float NewBaseMovementSpeed = Player->GetBaseMovementSpeed() + SpeedChangePerSecond * DeltaTime;
			Player->SetBaseMovementSpeed(NewBaseMovementSpeed);
		}
		else
		{
			Player->SetBaseMovementSpeed(Player->GetPlayerMovementData()->GetRunSpeed());
			bShouldUpdateSpeed = false;
		}
	}
}

void USuraPlayerJumpingState::UpdateState(ASuraCharacterPlayer* Player, float DeltaTime)
{
	Super::UpdateState(Player, DeltaTime);
	
	if (Player->bCrouchTriggered)
	{
		Player->InterpCapsuleHeight(0.6f, DeltaTime);
	}
	else
	{
		Player->InterpCapsuleHeight(1.f, DeltaTime);
	}

	UpdateBaseMovementSpeed(Player, DeltaTime);

	if (!bShouldCheckLedge)
	{
		if (ElapsedTimeFromHanging < 0.2f)
		{
			ElapsedTimeFromHanging += DeltaTime;
		}
		else
		{
			bShouldCheckLedge = true;
		}
	}

	if (bShouldCheckLedge)
	{
		FHitResult WallHitResult;
		FCollisionQueryParams WallParams;
		WallParams.AddIgnoredActor(Player);

		const FVector WallDetectStart = Player->GetActorLocation();
		const FVector WallDetectEnd = WallDetectStart + Player->GetActorForwardVector() * 35.f;
		const bool bWallHit = GetWorld()->SweepSingleByChannel(WallHitResult, WallDetectStart, WallDetectEnd, FQuat::Identity,
			ECC_GameTraceChannel2, FCollisionShape::MakeCapsule(34.f,
				Player->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() * 0.85f), WallParams);

		if (bWallHit && Player->GetCharacterMovement()->IsFalling() && WallHitResult.ImpactNormal.Z < Player->GetCharacterMovement()->GetWalkableFloorZ())
		{
			Player->WallHitResult = WallHitResult;
			FHitResult LedgeHitResult;
			FCollisionQueryParams LedgeParams;
			LedgeParams.AddIgnoredActor(Player);
		
			FVector LedgeDetectStart = FVector(WallHitResult.ImpactPoint.X, WallHitResult.ImpactPoint.Y,
				Player->GetCamera()->GetComponentLocation().Z + 100.f);
			FVector LedgeDetectEnd = FVector(LedgeDetectStart.X, LedgeDetectStart.Y, WallHitResult.ImpactPoint.Z);

			bool bLedgeHit = GetWorld()->SweepSingleByChannel(LedgeHitResult, LedgeDetectStart, LedgeDetectEnd, FQuat::Identity,
				ECC_GameTraceChannel2, FCollisionShape::MakeSphere(20.f), LedgeParams);

			if (bLedgeHit && Player->GetCharacterMovement()->IsWalkable(LedgeHitResult))
			{
			
				Player->LedgeHitResult = LedgeHitResult;
				if (LedgeHitResult.ImpactPoint.Z < Player->GetActorLocation().Z && Player->ForwardAxisInputValue > 0.f)
				{
					Player->ChangeState(Player->MantlingState);
					return;
				}
				Player->ChangeState(Player->HangingState);
				return;
			}
		}
	}

	

	if (Player->GetPreviousState()->GetStateType() == EPlayerState::WallRunning)
	{
		ElapsedTimeFromWallRun += DeltaTime;
		if (ElapsedTimeFromWallRun > 0.2f)
		{
			if (Player->ShouldEnterWallRunning(Player->WallRunDirection, Player->WallRunSide))
			{
				Player->ChangeState(Player->WallRunningState);
				return;
			}
		}
	}
	else
	{
		if (Player->ShouldEnterWallRunning(Player->WallRunDirection, Player->WallRunSide))
		{
			Player->ChangeState(Player->WallRunningState);
			return;
		}
	}

	if (Player->bJumpTriggered && Player->JumpsLeft > 0)
	{
		SlideSpeedDecreaseElapsedTime = 0.f;
		Player->JumpsLeft--;
		Player->InAirJump();
		Player->ChangeState(Player->JumpingState);
		return;
	}
	

	if (Player->IsFallingDown())
	{
		Player->ChangeState(Player->FallingState);
		return;
	}

	if (Player->bDashTriggered && Player->DashesLeft > 0)
	{
		Player->ChangeState(Player->DashingState);
		return;
	}

	if (Player->bLandedTriggered || Player->GetCharacterMovement()->IsMovingOnGround())
	{
		Player->ChangeState(Player->WalkingState);
		return;
	}
}

void USuraPlayerJumpingState::ExitState(ASuraCharacterPlayer* Player)
{
	Super::ExitState(Player);
	bShouldUpdateSpeed = false;
	ElapsedTimeFromWallRun = 0.f;
	ElapsedTimeFromHanging = 0.f;
}

void USuraPlayerJumpingState::Move(ASuraCharacterPlayer* Player, const FVector2D& InputVector)
{
	Super::Move(Player, InputVector);

	Player->AddMovementInput(Player->GetActorForwardVector(), InputVector.Y);
	Player->AddMovementInput(Player->GetActorRightVector(), InputVector.X);
}

void USuraPlayerJumpingState::Look(ASuraCharacterPlayer* Player, const FVector2D& InputVector)
{
	Super::Look(Player, InputVector);

	Player->AddControllerPitchInput(InputVector.Y);
	Player->AddControllerYawInput(InputVector.X);
}

void USuraPlayerJumpingState::SetSlideSpeedDecreaseElapsedTime(float ElapsedTime)
{
	SlideSpeedDecreaseElapsedTime = ElapsedTime;
}

