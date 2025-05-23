// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/SuraPlayerCrouchingState.h"

#include "ActorComponents/ACPlayerMovmentData.h"
#include "Camera/CameraComponent.h"
#include "Characters/Player/SuraCharacterPlayer.h"
#include "Characters/Player/SuraPlayerDashingState.h"
#include "Characters/Player/SuraPlayerFallingState.h"
#include "Characters/Player/SuraPlayerJumpingState.h"
#include "Characters/Player/SuraPlayerRunningState.h"
#include "Characters/Player/SuraPlayerWalkingState.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


USuraPlayerCrouchingState::USuraPlayerCrouchingState()
{
	StateDisplayName = "Crouching";
	StateType = EPlayerState::Crouching;
	
}

void USuraPlayerCrouchingState::EnterState(ASuraCharacterPlayer* Player)
{
	Super::EnterState(Player);

	bShouldUpdateSpeed = true;

	PlayerController = Player->GetController<APlayerController>();

	float CrouchSpeed = Player->GetPlayerMovementData()->GetCrouchSpeed();


	switch (Player->GetPreviousGroundedState()->GetStateType())
	{
	case EPlayerState::Walking:
		{
			SpeedTransitionTime = Player->GetPlayerMovementData()->GetWalkCrouchTransitionDuration();
			float WalkSpeed = Player->GetPlayerMovementData()->GetWalkSpeed();
			SpeedChangePerSecond = (CrouchSpeed - WalkSpeed) / SpeedTransitionTime;
			break;
		}

	case EPlayerState::Dashing:
		{
			SpeedTransitionTime = Player->GetPlayerMovementData()->GetCrouchDashTransitionDuration();
			float DashEndSpeed = Player->GetPlayerMovementData()->GetDashEndSpeed();
			SpeedChangePerSecond = (CrouchSpeed- DashEndSpeed) / SpeedTransitionTime;
			break;
		}
	case EPlayerState::Running:
		{
			SpeedTransitionTime = Player->GetPlayerMovementData()->GetCrouchRunTransitionDuration();
			float RunSpeed = Player->GetPlayerMovementData()->GetRunSpeed();
			SpeedChangePerSecond = (CrouchSpeed - RunSpeed) / SpeedTransitionTime;
			break;
		}
	case EPlayerState::Sliding:
		{
			bShouldUpdateSpeed = false;
		}
	default:
		break;
	}
	
	
}

void USuraPlayerCrouchingState::UpdateState(ASuraCharacterPlayer* Player, float DeltaTime)
{
	Super::UpdateState(Player, DeltaTime);

	Player->InterpCapsuleHeight(0.6f, DeltaTime);

	Player->StartCamShake(Player->CrouchCamShake);

	FHitResult UpperHit;
	FCollisionQueryParams UpperParams;
	UpperParams.AddIgnoredActor(Player);
	FVector Start = Player->GetCharacterMovement()->CurrentFloor.HitResult.Location + 30.f;
	FVector End = Start + FVector::UpVector * Player->GetDefaultCapsuleHalfHeight() * 2.f - 30.f;
	bool bUpperHit = GetWorld()->SweepSingleByChannel(UpperHit, Start, End, FQuat::Identity, ECC_Visibility,
		FCollisionShape::MakeSphere(50.f), UpperParams);


	if (bShouldUpdateSpeed)
	{
		float CurrentBaseSpeed = Player->GetBaseMovementSpeed();
		float TargetCrouchSpeed = Player->GetPlayerMovementData()->GetCrouchSpeed();
		if (SpeedChangePerSecond > 0 && CurrentBaseSpeed < TargetCrouchSpeed ||
			SpeedChangePerSecond < 0 && CurrentBaseSpeed > TargetCrouchSpeed)
		{
			float NewBaseMovementSpeed = Player->GetBaseMovementSpeed() + SpeedChangePerSecond * DeltaTime;
			Player->SetBaseMovementSpeed(NewBaseMovementSpeed);
		}
		else
		{
			Player->SetBaseMovementSpeed(Player->GetPlayerMovementData()->GetCrouchSpeed());
			bShouldUpdateSpeed = false;
		}
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
	
	if (Player->bJumpTriggered && Player->JumpsLeft > 0)
	{
		Player->JumpsLeft--;
		Player->PrimaryJump();
		Player->ChangeState(Player->JumpingState);
		return;
	}

	if (!Player->bCrouchTriggered && !bUpperHit)
	{
		Player->ChangeState(Player->RunningState);
		return;
	}

	
}

void USuraPlayerCrouchingState::ExitState(ASuraCharacterPlayer* Player)
{
	Super::ExitState(Player);

	Player->SetPreviousGroundedState(Player->CrouchingState);
	bShouldUpdateSpeed = false;
	
}

void USuraPlayerCrouchingState::Move(ASuraCharacterPlayer* Player, const FVector2D& InputVector)
{
	Player->AddMovementInput(Player->GetActorForwardVector(), InputVector.Y);
	Player->AddMovementInput(Player->GetActorRightVector(), InputVector.X);
}

void USuraPlayerCrouchingState::Look(ASuraCharacterPlayer* Player, const FVector2D& InputVector)
{
	Player->AddControllerPitchInput(InputVector.Y);
	Player->AddControllerYawInput(InputVector.X);
}





