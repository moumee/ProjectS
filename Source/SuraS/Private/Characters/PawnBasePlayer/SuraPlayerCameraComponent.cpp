// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PawnBasePlayer/SuraPlayerCameraComponent.h"
#include "Characters/PawnBasePlayer/PlayerCameraMovementRow.h"
#include "Characters/PawnBasePlayer/SuraPawnPlayer.h"
#include "Characters/PawnBasePlayer/SuraPlayerMovementComponent.h"

// Sets default values for this component's properties
USuraPlayerCameraComponent::USuraPlayerCameraComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USuraPlayerCameraComponent::BeginPlay()
{
	Super::BeginPlay();

	ASuraPawnPlayer* Player = GetOwner<ASuraPawnPlayer>();
	check(Player);
	MovementComponent = Player->GetPlayerMovementComponent();
	PlayerCamera = Player->GetCameraComponent();
	PlayerController = Player->GetController<APlayerController>();

	InitCameraShakes();

	MovementComponent->OnMove.AddUObject(this, &USuraPlayerCameraComponent::OnMove);
	MovementComponent->OnAirborne.AddUObject(this, &USuraPlayerCameraComponent::OnAirborne);
	MovementComponent->OnSlide.AddUObject(this, &USuraPlayerCameraComponent::OnSlide);
	MovementComponent->OnWallRun.AddUObject(this, &USuraPlayerCameraComponent::OnWallRun);
	
	MovementComponent->OnLand.AddUObject(this, &USuraPlayerCameraComponent::OnLand);
	MovementComponent->OnPrimaryJump.AddUObject(this, &USuraPlayerCameraComponent::OnPrimaryJump);
	MovementComponent->OnDoubleJump.AddUObject(this, &USuraPlayerCameraComponent::OnDoubleJump);
	MovementComponent->OnWallJump.AddUObject(this, &USuraPlayerCameraComponent::OnWallJump);
	MovementComponent->OnMantle.AddUObject(this, &USuraPlayerCameraComponent::OnMantle);
	MovementComponent->OnDash.AddUObject(this, &USuraPlayerCameraComponent::OnDash);
	

	
	
}

void USuraPlayerCameraComponent::ChangeCameraLoopShake(const TSubclassOf<UCameraShakeBase>& InShake)
{
	if (CurrentLoopShake)
	{
		PlayerController->PlayerCameraManager->StopAllInstancesOfCameraShake(CurrentLoopShake, false);
	}
	CurrentLoopShake = InShake;
	PlayerController->PlayerCameraManager->StartCameraShake(InShake);

}

void USuraPlayerCameraComponent::PlayOneShotCameraShake(const TSubclassOf<UCameraShakeBase>& InShake)
{
	PlayerController->PlayerCameraManager->StartCameraShake(InShake);
}


void USuraPlayerCameraComponent::TickMoveStateLoopShake()
{
	if (bIsMoveState && MovementComponent->GetMovementState() == EMovementState::EMS_Move)
	{
		if (MovementComponent->IsCrouching())
		{
			if (MovementComponent->Velocity.IsNearlyZero(0.001))
			{
				if (CurrentLoopShake != CrouchIdleCameraShake)
				{
					ChangeCameraLoopShake(CrouchIdleCameraShake);
				}
			}
			else
			{
				if (CurrentLoopShake != CrouchMoveCameraShake)
				{
					ChangeCameraLoopShake(CrouchMoveCameraShake);
				}
			}
		}
		else
		{
			if (MovementComponent->Velocity.IsNearlyZero(0.001))
			{
				if (CurrentLoopShake != IdleCameraShake)
				{
					ChangeCameraLoopShake(IdleCameraShake);
				}
			}
			else if (MovementComponent->GetMovementInputVector().Y > 0.f)
			{
				if (CurrentLoopShake != RunCameraShake)
				{
					ChangeCameraLoopShake(RunCameraShake);
				}
			}
			else
			{
				if (CurrentLoopShake != WallRunCameraShake)
				{
					ChangeCameraLoopShake(WalkCameraShake);
				}
			}
		}
	}
}

// Called every frame
void USuraPlayerCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	TickMoveStateLoopShake();
	
	
}

void USuraPlayerCameraComponent::OnAirborne()
{
	bIsMoveState = false;
	ChangeCameraLoopShake(AirborneCameraShake);
	
}

void USuraPlayerCameraComponent::OnMove()
{
	bIsMoveState = true;
}

void USuraPlayerCameraComponent::OnWallRun()
{
	bIsMoveState = false;
	PlayOneShotCameraShake(WallLandCameraShake);
	ChangeCameraLoopShake(WallRunCameraShake);
}

void USuraPlayerCameraComponent::OnSlide()
{
	bIsMoveState = false;
	ChangeCameraLoopShake(SlideCameraShake);
}

void USuraPlayerCameraComponent::OnMantle()
{
	bIsMoveState = false;
	PlayOneShotCameraShake(MantleCameraShake);
}


void USuraPlayerCameraComponent::OnPrimaryJump()
{
	PlayOneShotCameraShake(PrimaryJumpCameraShake);	
}

void USuraPlayerCameraComponent::OnDoubleJump()
{
	PlayOneShotCameraShake(DoubleJumpCameraShake);
}

void USuraPlayerCameraComponent::OnWallJump()
{
	PlayOneShotCameraShake(WallJumpCameraShake);	
}

void USuraPlayerCameraComponent::OnLand(float ZVelocity)
{
	if (ZVelocity < -300.f)
	{
		PlayOneShotCameraShake(StrongLandCameraShake);
	}
	else if (ZVelocity < -150.f)
	{
		PlayOneShotCameraShake(MediumLandCameraShake);
	}
	else if (ZVelocity < 0.f)
	{
		PlayOneShotCameraShake(LightLandCameraShake);
	}
}





void USuraPlayerCameraComponent::OnDash(FVector2D MovementInput)
{
	if (MovementInput.IsNearlyZero(0.001))
	{
		PlayOneShotCameraShake(ForwardDashCameraShake);
	}
	else if (MovementInput.Y > 0.f)
	{
		if (MovementInput.X > 0.f)
		{
			PlayOneShotCameraShake(ForwardRightDashCameraShake);
		}
		else if (MovementInput.X < 0.f)
		{
			PlayOneShotCameraShake(ForwardLeftDashCameraShake);
		}
		else
		{
			PlayOneShotCameraShake(ForwardDashCameraShake);
		}
	}
	else if (MovementInput.Y < 0.f)
	{
		if (MovementInput.X > 0.f)
		{
			PlayOneShotCameraShake(BackwardRightDashCameraShake);
		}
		else if (MovementInput.X < 0.f)
		{
			PlayOneShotCameraShake(BackwardLeftDashCameraShake);
		}
		else
		{
			PlayOneShotCameraShake(BackwardDashCameraShake);
		}
	}
	else
	{
		if (MovementInput.X > 0.f)
		{
			PlayOneShotCameraShake(RightDashCameraShake);
		}
		else if (MovementInput.X < 0.f)
		{
			PlayOneShotCameraShake(LeftDashCameraShake);
		}
		else
		{
			PlayOneShotCameraShake(ForwardDashCameraShake);
		}
	}
	
}

void USuraPlayerCameraComponent::InitCameraShakes()
{
	FPlayerCameraMovementRow* Row = CameraDataTable->FindRow<FPlayerCameraMovementRow>("Player", "");
	if (!Row) return;

	IdleCameraShake = Row->IdleCameraShake;
	WalkCameraShake = Row->WalkCameraShake;
	RunCameraShake = Row->RunCameraShake;
	CrouchIdleCameraShake = Row->CrouchIdleCameraShake;
	CrouchMoveCameraShake = Row->CrouchMoveCameraShake;
	AirborneCameraShake = Row->AirborneCameraShake;
	WallRunCameraShake = Row->WallRunCameraShake;
	SlideCameraShake = Row->SlideCameraShake;
	PrimaryJumpCameraShake = Row->PrimaryJumpCameraShake;
	DoubleJumpCameraShake = Row->DoubleJumpCameraShake;
	WallJumpCameraShake = Row->WallJumpCameraShake;
	LightLandCameraShake = Row->LightLandCameraShake;
	MediumLandCameraShake = Row->MediumLandCameraShake;
	StrongLandCameraShake = Row->StrongLandCameraShake;
	WallLandCameraShake = Row->WallLandCameraShake;
	MantleCameraShake = Row->MantleCameraShake;
	ForwardDashCameraShake = Row->ForwardDashCameraShake;
	ForwardRightDashCameraShake = Row->ForwardRightDashCameraShake;
	ForwardLeftDashCameraShake = Row->ForwardLeftDashCameraShake;
	LeftDashCameraShake = Row->LeftDashCameraShake;
	RightDashCameraShake = Row->RightDashCameraShake;
	BackwardDashCameraShake = Row->BackwardDashCameraShake;
	BackwardRightDashCameraShake = Row->BackwardRightDashCameraShake;
	BackwardLeftDashCameraShake = Row->BackwardLeftDashCameraShake;
}






