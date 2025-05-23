// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/SuraPlayerWalkingState.h"

#include "ActorComponents/ACPlayerMovmentData.h"
#include "ActorComponents/WeaponSystem/ACWeapon.h"
#include "ActorComponents/WeaponSystem/SuraWeaponBaseState.h"
#include "ActorComponents/WeaponSystem/WeaponStateType.h"
#include "ActorComponents/WeaponSystem/WeaponSystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Characters/Player/SuraCharacterPlayer.h"
#include "Characters/Player/SuraPlayerCrouchingState.h"
#include "Characters/Player/SuraPlayerDashingState.h"
#include "Characters/Player/SuraPlayerFallingState.h"
#include "Characters/Player/SuraPlayerJumpingState.h"
#include "Characters/Player/SuraPlayerRunningState.h"
#include "Components/CapsuleComponent.h"

USuraPlayerWalkingState::USuraPlayerWalkingState()
{
	StateDisplayName = "Walking";
	StateType = EPlayerState::Walking;
}


void USuraPlayerWalkingState::EnterState(ASuraCharacterPlayer* Player)
{
	Super::EnterState(Player);

	PlayerController = Player->GetController<APlayerController>();

	bShouldUpdateSpeed = true;

	float WalkSpeed = Player->GetPlayerMovementData()->GetWalkSpeed();
	
	switch (Player->GetPreviousGroundedState()->GetStateType())
	{
	case EPlayerState::Running:
		{
			SpeedTransitionTime = Player->GetPlayerMovementData()->GetWalkRunTransitionDuration();
			float RunSpeed = Player->GetPlayerMovementData()->GetRunSpeed();
			SpeedChangePerSecond = (WalkSpeed - RunSpeed) / SpeedTransitionTime;
			break;
		}
	case EPlayerState::Dashing:
		{
			SpeedTransitionTime = Player->GetPlayerMovementData()->GetWalkDashTransitionDuration();
			float DashEndSpeed = Player->GetPlayerMovementData()->GetDashEndSpeed();
			SpeedChangePerSecond = (WalkSpeed - DashEndSpeed) / SpeedTransitionTime;
			break;
		}
	case EPlayerState::Crouching:
		{
			SpeedTransitionTime = Player->GetPlayerMovementData()->GetWalkCrouchTransitionDuration();
			float CrouchSpeed = Player->GetPlayerMovementData()->GetCrouchSpeed();
			SpeedChangePerSecond = (WalkSpeed - CrouchSpeed) / SpeedTransitionTime;
			break;
		}
	default:
		break;
	}
}

void USuraPlayerWalkingState::UpdateState(ASuraCharacterPlayer* Player, float DeltaTime)
{
	Super::UpdateState(Player, DeltaTime);

	Player->InterpCapsuleHeight(1.f, DeltaTime);

	if (!Player->HasMovementInput())
	{
		Player->StartCamShake(Player->IdleCamShake);
	}
	else
	{
		Player->StartCamShake(Player->WalkCamShake);
	}

	if (bShouldUpdateSpeed)
	{
		float CurrentBaseSpeed = Player->GetBaseMovementSpeed();
		float TargetWalkSpeed = Player->GetPlayerMovementData()->GetWalkSpeed();
		if (SpeedChangePerSecond > 0 && CurrentBaseSpeed < TargetWalkSpeed ||
			SpeedChangePerSecond < 0 && CurrentBaseSpeed > TargetWalkSpeed)
		{
			float NewBaseMovementSpeed = Player->GetBaseMovementSpeed() + SpeedChangePerSecond * DeltaTime;
			Player->SetBaseMovementSpeed(NewBaseMovementSpeed);
		}
		else
		{
			Player->SetBaseMovementSpeed(Player->GetPlayerMovementData()->GetWalkSpeed());
			bShouldUpdateSpeed = false;
		}
	}

	if (Player->IsFallingDown())
	{
		Player->ChangeState(Player->FallingState);
		return;
	}

	if (Player->ForwardAxisInputValue > 0.f)
	{
		if (Player->HasWeapon())
		{
			if (Player->GetWeaponSystemComponent()->GetCurrentWeapon()->GetCurrentState()->GetWeaponStateType() != EWeaponStateType::WeaponStateType_Charging &&
				!Player->GetWeaponSystemComponent()->IsZoomIn())
			{
				Player->ChangeState(Player->RunningState);
				return;
			}
		}
		else
		{
			Player->ChangeState(Player->RunningState);
			return;
		}
	}
	

	if (Player->bCrouchTriggered)
	{
		Player->ChangeState(Player->CrouchingState);
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
}

void USuraPlayerWalkingState::ExitState(ASuraCharacterPlayer* Player)
{
	Super::ExitState(Player);

	Player->SetPreviousGroundedState(this);
	bShouldUpdateSpeed = false;
}

void USuraPlayerWalkingState::Move(ASuraCharacterPlayer* Player, const FVector2D& InputVector)
{
	Super::Move(Player, InputVector);

	Player->AddMovementInput(Player->GetActorForwardVector(), InputVector.Y);
	Player->AddMovementInput(Player->GetActorRightVector(), InputVector.X);
	
}

void USuraPlayerWalkingState::Look(ASuraCharacterPlayer* Player, const FVector2D& InputVector)
{
	Super::Look(Player, InputVector);

	Player->AddControllerPitchInput(InputVector.Y);
	Player->AddControllerYawInput(InputVector.X);
}

