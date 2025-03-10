// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/SuraPlayerSlidingState.h"

#include "ActorComponents/ACPlayerMovmentData.h"
#include "Characters/Player/SuraCharacterPlayer.h"
#include "Characters/Player/SuraPlayerCrouchingState.h"
#include "Characters/Player/SuraPlayerDashingState.h"
#include "Characters/Player/SuraPlayerFallingState.h"
#include "Characters/Player/SuraPlayerJumpingState.h"
#include "Characters/Player/SuraPlayerRunningState.h"
#include "Characters/Player/SuraPlayerWalkingState.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

USuraPlayerSlidingState::USuraPlayerSlidingState()
{
	StateDisplayName = "Sliding State";
	StateType = EPlayerState::Sliding;
}

void USuraPlayerSlidingState::EnterState(ASuraCharacterPlayer* Player)
{
	Super::EnterState(Player);

	PlayerController = Player->GetController<APlayerController>();

	Player->GetCharacterMovement()->GroundFriction = 0.f;
	Player->GetCharacterMovement()->BrakingDecelerationWalking = 0.f;

	StateEnterSpeed = Player->GetCharacterMovement()->Velocity.Size();

	CrouchSpeed = Player->GetPlayerMovementData()->GetCrouchSpeed();

	SlideAdditionalSpeed = Player->GetPlayerMovementData()->GetSlidingAdditionalSpeed();
	SlideSpeedIncreaseTime = Player->GetPlayerMovementData()->GetSlidingSpeedIncreaseTime();
	SlideSpeedDecreaseTime = Player->GetPlayerMovementData()->GetSlidingSpeedDecreaseTime();

	bSpeedIncreasing = true;
	

	if (Player->GetPreviousState()->GetStateType() == EPlayerState::Falling)
	{
		FVector DesiredSlidingDirection = Player->FallingState->GetDesiredSlidingDirection();
		SlideDirection = FVector::VectorPlaneProject(DesiredSlidingDirection,
			Player->GetCharacterMovement()->CurrentFloor.HitResult.ImpactNormal).GetSafeNormal();
	}
	else
	{
		FVector ForwardVector = Player->GetActorForwardVector();
		FVector RightVector = Player->GetActorRightVector();
		FVector SlideDirectionXY = (ForwardVector * Player->ForwardAxisInputValue + RightVector * Player->RightAxisInputValue).GetSafeNormal();
		SlideDirection = FVector::VectorPlaneProject(SlideDirectionXY, Player->GetCharacterMovement()->CurrentFloor.HitResult.ImpactNormal).GetSafeNormal();
	}

	if (!FMath::IsNearlyEqual(Player->FindFloorAngle(), 0.f, 0.1f))
	{
		CachedSlopeDownwardDirection = FVector::VectorPlaneProject(FVector::DownVector, Player->GetCharacterMovement()->CurrentFloor.HitResult.ImpactNormal).GetSafeNormal();
	}

	Player->GetCharacterMovement()->Velocity = SlideDirection * StateEnterSpeed;
	
}

void USuraPlayerSlidingState::UpdateState(ASuraCharacterPlayer* Player, float DeltaTime)
{
	Super::UpdateState(Player, DeltaTime);

	Player->StartCamShake(Player->SlideCamShake);

	FHitResult UpperHit;
	FCollisionQueryParams UpperParams;
	UpperParams.AddIgnoredActor(Player);
	FVector Start = Player->GetCharacterMovement()->CurrentFloor.HitResult.Location;
	FVector End = Start + FVector::UpVector * Player->GetDefaultCapsuleHalfHeight() * 2.f + 30.f;
	bool bUpperHit = GetWorld()->SweepSingleByChannel(UpperHit, Start, End, FQuat::Identity, ECC_Visibility,
		FCollisionShape::MakeSphere(50.f), UpperParams);

	
	

	if (bSpeedIncreasing)
	{
		if (IncreaseElapsedTime < SlideSpeedIncreaseTime)
		{
			IncreaseElapsedTime += DeltaTime;
			CurrentSlideSpeed = FMath::Lerp(StateEnterSpeed, StateEnterSpeed + SlideAdditionalSpeed, IncreaseElapsedTime / SlideSpeedIncreaseTime);
		}
		else
		{
			CurrentSlideSpeed = StateEnterSpeed + SlideAdditionalSpeed;
			bSpeedIncreasing = false;
		}
	}
	else
	{
		if (Player->FindFloorAngle() >= -0.1f)
		{
			if (DecreaseElapsedTime < SlideSpeedDecreaseTime)
			{
				DecreaseElapsedTime += DeltaTime;
				CurrentSlideSpeed = FMath::Lerp(StateEnterSpeed + SlideAdditionalSpeed, CrouchSpeed, DecreaseElapsedTime / SlideSpeedDecreaseTime);
			}
			else
			{
				CurrentSlideSpeed = CrouchSpeed;
			}
		}
	}
	

	if (Player->FindFloorAngle() >= -0.1f)
	{
		//TODO: I need to start acceleration if I am back on slope
	}

	if (FMath::IsNearlyEqual(Player->FindFloorAngle(), 0.f, 0.1f))
	{
		TargetSlideDirection = SlideDirection;
	}
	else
	{
		FVector CurrentSlopeDownwardDirection = FVector::VectorPlaneProject(FVector::DownVector,
		Player->GetCharacterMovement()->CurrentFloor.HitResult.ImpactNormal).GetSafeNormal();
		// if (FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(CurrentSlopeDownwardDirection, CachedSlopeDownwardDirection))) > 5.f)
		// {
		// 	ElapsedTime = 0.f;
		// }
		if (Player->FindFloorAngle() < 0.f)
		{
			TargetSlideDirection = FVector::VectorPlaneProject(
				FVector::DownVector, Player->GetCharacterMovement()->CurrentFloor.HitResult.ImpactNormal).GetSafeNormal();
		}
		else if (Player->FindFloorAngle() > 0.f)
		{
			TargetSlideDirection = FVector::VectorPlaneProject(
				FVector::UpVector, Player->GetCharacterMovement()->CurrentFloor.HitResult.ImpactNormal).GetSafeNormal();
		}
	}
	
	

	Player->InterpCapsuleHeight(0.6f, DeltaTime);
	

	FVector NewSlideDirection = FMath::VInterpNormalRotationTo(Player->GetCharacterMovement()->Velocity.GetSafeNormal(), TargetSlideDirection, DeltaTime, 40.f);
	Player->GetCharacterMovement()->Velocity = NewSlideDirection * CurrentSlideSpeed;
	

	if (CurrentSlideSpeed <= CrouchSpeed)
	{
		Player->SetBaseMovementSpeed(CrouchSpeed);
		Player->ChangeState(Player->CrouchingState);
		return;
	}

	// Only for sliding jumping because of the capsule size reset
	// The order matters
	if (Player->bJumpTriggered)
	{
		if (Player->JumpsLeft > 0)
		{
			Player->JumpsLeft--;
			FVector ForwardVector = Player->GetActorForwardVector();
			FVector JumpVector = ForwardVector * Player->GetPlayerMovementData()->GetDoubleJumpXYSpeed() * 2.f +
				FVector::UpVector * Player->GetPlayerMovementData()->GetPrimaryJumpZSpeed();
			Player->LaunchCharacter(JumpVector, false, true);
		}
		Player->ChangeState(Player->JumpingState);
		return;
		
	}

	if (Player->IsFallingDown() || !Player->GetCharacterMovement()->CurrentFloor.bBlockingHit)
	{
		Player->ChangeState(Player->FallingState);
		return;
	}

	if (Player->bDashTriggered)
	{
		Player->ChangeState(Player->DashingState);
		return;
	}

	

	if (!Player->bCrouchTriggered)
	{
		if (bUpperHit)
		{
			Player->ChangeState(Player->CrouchingState);
			return;
		}
		Player->ChangeState(Player->WalkingState);
		return;
	}
}

void USuraPlayerSlidingState::ExitState(ASuraCharacterPlayer* Player)
{
	Super::ExitState(Player);
	Player->GetCharacterMovement()->GroundFriction = Player->DefaultGroundFriction;
	Player->GetCharacterMovement()->BrakingDecelerationWalking = Player->DefaultBrakingDecelerationWalking;
	Player->SetPreviousGroundedState(this);
	IncreaseElapsedTime = 0.f;
	DecreaseElapsedTime = 0.f;
}


void USuraPlayerSlidingState::StartJumping(ASuraCharacterPlayer* Player)
{
	Super::StartJumping(Player);
}

void USuraPlayerSlidingState::Look(ASuraCharacterPlayer* Player, const FVector2D& InputVector)
{
	Super::Look(Player, InputVector);
	Player->AddControllerPitchInput(InputVector.Y);
	Player->AddControllerYawInput(InputVector.X);
}


