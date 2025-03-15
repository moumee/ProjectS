// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PawnBasePlayer/SuraPawnPlayer.h"

#include "AITypes.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Characters/PawnBasePlayer/SuraPawnPlayerMovementComponent.h"
#include "Components/CapsuleComponent.h"

ASuraPawnPlayer::ASuraPawnPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Component"));
	RootComponent = CapsuleComponent;
	CapsuleComponent->SetSimulatePhysics(false);
	CapsuleComponent->SetCapsuleSize(40.f, 90.f);
	CapsuleComponent->SetCollisionProfileName(TEXT("Pawn"));
	CapsuleComponent->SetNotifyRigidBodyCollision(true);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CapsuleComponent);
	Camera->SetRelativeLocation(FVector(0.f, 0.f, 70.f));
	Camera->bUsePawnControlRotation = true;

	ArmMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Arm Mesh"));
	ArmMesh->SetupAttachment(Camera);

	MovementComponent = CreateDefaultSubobject<USuraPawnPlayerMovementComponent>(TEXT("Movement Component"));

	AutoPossessPlayer = EAutoReceiveInput::Player0;

}

void ASuraPawnPlayer::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}


void ASuraPawnPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASuraPawnPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASuraPawnPlayer::StartMove);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &ASuraPawnPlayer::StopMove);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASuraPawnPlayer::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ASuraPawnPlayer::StartJump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ASuraPawnPlayer::StopJump);
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Triggered, this, &ASuraPawnPlayer::StartDash);
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Completed, this, &ASuraPawnPlayer::StopDash);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &ASuraPawnPlayer::StartCrouch);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &ASuraPawnPlayer::StopCrouch);
	}

}

void ASuraPawnPlayer::StartMove(const FInputActionValue& Value)
{
	FVector2D InputVector = Value.Get<FVector2D>();
	
	if (MovementComponent)
	{
		MovementComponent->SetMoveInput(InputVector);
	}
	
}

void ASuraPawnPlayer::StopMove()
{
	if (MovementComponent)
	{
		MovementComponent->SetMoveInput(FVector2D::ZeroVector);
	}
}


void ASuraPawnPlayer::Look(const FInputActionValue& Value)
{
	FVector2D InputVector = Value.Get<FVector2D>();

	AddControllerYawInput(InputVector.X);
	AddControllerPitchInput(InputVector.Y);
}

void ASuraPawnPlayer::StartJump()
{
	MovementComponent->SetJumpInput(true);
}

void ASuraPawnPlayer::StopJump()
{
	MovementComponent->SetJumpInput(false);
}

void ASuraPawnPlayer::StartDash()
{
	MovementComponent->SetDashInput(true);
}

void ASuraPawnPlayer::StopDash()
{
	MovementComponent->SetDashInput(false);
}

void ASuraPawnPlayer::StartCrouch()
{
	MovementComponent->SetCrouchInput(true);
}

void ASuraPawnPlayer::StopCrouch()
{
	MovementComponent->SetCrouchInput(false);
}



