// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/SuraPlayerDashingState.h"

#include "ActorComponents/ACPlayerMovmentData.h"
#include "Camera/CameraComponent.h"
#include "Characters/Player/SuraCharacterPlayer.h"
#include "Characters/Player/SuraPlayerFallingState.h"
#include "Characters/Player/SuraPlayerJumpingState.h"
#include "Characters/Player/SuraPlayerRunningState.h"
#include "Characters/Player/SuraPlayerWalkingState.h"
#include "Characters/Player/SuraPlayerWallRunningState.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

USuraPlayerDashingState::USuraPlayerDashingState()
{
	StateDisplayName = "Dashing";
	StateType = EPlayerState::Dashing;
}


void USuraPlayerDashingState::EnterState(ASuraCharacterPlayer* Player)
{
	
	Super::EnterState(Player);

	PlayerController = Player->GetController<APlayerController>();
	
	DashElapsedTime = 0.f;
	

	Player->DashesLeft--;
	Player->JumpsLeft = Player->MaxJumps - 1;
	
	for (int i = 0; i < Player->DashCooldowns.Num(); i++)
	{
		if (Player->DashCooldowns[i] == 0)
		{
			Player->DashCooldowns[i] = Player->GetPlayerMovementData()->GetDashCooldown();
			break;
		}
	}

	const float NewBaseMovementSpeed = Player->GetPlayerMovementData()->GetDashSpeed();
	Player->SetBaseMovementSpeed(NewBaseMovementSpeed);
	
	if (FMath::IsNearlyEqual(Player->RightAxisInputValue, 0.f))
	{
		if (Player->ForwardAxisInputValue >= 0.f)
		{
			DashDirection = Player->GetCamera()->GetForwardVector();
		}
		else
		{
			DashDirection = Player->GetCamera()->GetForwardVector() * -1.f;
		}
		
	}
	else
	{
		DashDirection = (Player->GetActorForwardVector() * Player->ForwardAxisInputValue +
			Player->GetActorRightVector() * Player->RightAxisInputValue).GetSafeNormal();
		
		if (Player->ForwardAxisInputValue < 0.f)
		{
			DashDirection.Z = Player->GetCamera()->GetForwardVector().Z * -1.f;
		}
	}
	
	DashImpulseSpeed = Player->GetPlayerMovementData()->GetDashImpulseSpeed();
	DashEndSpeed = Player->GetPlayerMovementData()->GetDashSpeed();

	EndTransitionDuration = Player->GetPlayerMovementData()->GetDashImpulseTransitionEndDuration();

	float DashImpulseDistance = Player->GetPlayerMovementData()->GetDashDistance();
	DashImpulseDuration = DashImpulseDistance / DashImpulseSpeed;
	Player->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
	Player->GetCharacterMovement()->Velocity = DashDirection * DashImpulseSpeed;

	Player->GetCharacterMovement()->BrakingFriction = 0.f;
	Player->GetCharacterMovement()->GroundFriction = 0.f;
	Player->GetCharacterMovement()->FallingLateralFriction = 0.f;
	Player->GetCharacterMovement()->BrakingDecelerationFalling = 0.f;
	Player->GetCharacterMovement()->BrakingDecelerationWalking = 0.f;
	
}

void USuraPlayerDashingState::UpdateState(ASuraCharacterPlayer* Player, float DeltaTime)
{
	Super::UpdateState(Player, DeltaTime);
	
	Player->InterpCapsuleHeight(1.f, DeltaTime);

	Player->StartCamShake(Player->DashCamShake);

	if (Player->ShouldEnterWallRunning(Player->WallRunDirection, Player->WallRunSide))
	{
		Player->ChangeState(Player->WallRunningState);
		return;
	}

	TArray<FHitResult> Hits;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Player);
	bool bHit = GetWorld()->LineTraceMultiByChannel(Hits, Player->GetActorLocation(), Player->GetActorLocation() + DashDirection * 50.f,
		ECC_Visibility, Params);

	if (bHit)
	{
		for (const FHitResult& Hit : Hits)
		{
			if (Hit.ImpactNormal.Z < Player->GetCharacterMovement()->GetWalkableFloorZ())
			{
				if (Player->ShouldEnterWallRunning(Player->WallRunDirection, Player->WallRunSide))
				{
					Player->ChangeState(Player->WallRunningState);
					return;
				}
				
				if (Player->GetCharacterMovement()->IsFalling())
				{
					if (Player->IsFallingDown())
					{
						Player->ChangeState(Player->FallingState);
						return;
					}
					Player->ChangeState(Player->JumpingState);
					return;
				}
				Player->ChangeState(Player->WalkingState);
				return;
			}
		}
		
	}
	
	
	if (DashElapsedTime < DashImpulseDuration + EndTransitionDuration)
	{
		DashElapsedTime += DeltaTime;
		if (DashElapsedTime >= DashImpulseDuration)
		{
			float Alpha = (DashElapsedTime - DashImpulseDuration) / EndTransitionDuration;
			float TargetSpeed = FMath::Lerp(DashImpulseSpeed, DashEndSpeed, Alpha);
			float NewSpeed = FMath::FInterpTo(Player->GetCharacterMovement()->Velocity.Size(), TargetSpeed, DeltaTime, 10.f);
			Player->GetCharacterMovement()->Velocity = NewSpeed * DashDirection;
		}
		
	}
	else
	{
		const FVector CurrentVelocity = Player->GetCharacterMovement()->Velocity;
		FVector ResetVelocity = CurrentVelocity.GetSafeNormal() * Player->GetPlayerMovementData()->GetDashSpeed();
		Player->GetCharacterMovement()->Velocity = ResetVelocity;
		
		if (Player->GetCharacterMovement()->IsFalling())
		{
			if (Player->IsFallingDown())
			{
				Player->ChangeState(Player->FallingState);
				return;
			}
			Player->ChangeState(Player->JumpingState);
			return;
		}
		Player->ChangeState(Player->RunningState);
		return;
	}

	
}

void USuraPlayerDashingState::ExitState(ASuraCharacterPlayer* Player)
{
	Super::ExitState(Player);
	Player->GetCharacterMovement()->BrakingFriction = Player->DefaultBrakingFriction;
	Player->GetCharacterMovement()->GroundFriction = Player->DefaultGroundFriction;
	Player->GetCharacterMovement()->FallingLateralFriction = Player->DefaultFallingLateralFriction;
	Player->GetCharacterMovement()->BrakingDecelerationFalling = Player->DefaultBrakingDecelerationFalling;
	Player->GetCharacterMovement()->BrakingDecelerationWalking = Player->DefaultBrakingDecelerationWalking;
	Player->SetPreviousGroundedState(this);
	DashElapsedTime = 0.f;
}

void USuraPlayerDashingState::Look(ASuraCharacterPlayer* Player, const FVector2D& InputVector)
{
	Super::Look(Player, InputVector);
	Player->AddControllerYawInput(InputVector.X);
	Player->AddControllerPitchInput(InputVector.Y);
}

