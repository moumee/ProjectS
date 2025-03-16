// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PawnBasePlayer/SuraPawnPlayer.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Characters/PawnBasePlayer/SuraPlayerMovementComponent.h"
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

	MovementComponent = CreateDefaultSubobject<USuraPlayerMovementComponent>(TEXT("Movement Component"));
	MovementComponent->UpdatedComponent = RootComponent;

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

UCapsuleComponent* ASuraPawnPlayer::GetCapsuleComponent()
{
	return CapsuleComponent;
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
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASuraPawnPlayer::HandleMoveInput);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &ASuraPawnPlayer::HandleMoveInput);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASuraPawnPlayer::HandleLookInput);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ASuraPawnPlayer::HandleJumpInput);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ASuraPawnPlayer::HandleJumpInput);
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Triggered, this, &ASuraPawnPlayer::HandleDashInput);
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Completed, this, &ASuraPawnPlayer::HandleDashInput);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &ASuraPawnPlayer::HandleCrouchInput);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &ASuraPawnPlayer::HandleCrouchInput);
	}

}

void ASuraPawnPlayer::HandleMoveInput(const FInputActionValue& Value)
{

	if (!MovementComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Player movement component is not valid!"));
		return;
	}
	
	FVector2D InputVector = Value.Get<FVector2D>();

	const FRotator YawRotation = FRotator(0, GetControlRotation().Yaw, 0);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
	MovementComponent->AddInputVector(ForwardDirection * InputVector.Y);
	MovementComponent->AddInputVector(RightDirection * InputVector.X);
	MovementComponent->SetMovementInputVector(InputVector);
}

void ASuraPawnPlayer::HandleLookInput(const FInputActionValue& Value)
{
	FVector2D InputVector = Value.Get<FVector2D>();

	AddControllerYawInput(InputVector.X);
	AddControllerPitchInput(InputVector.Y);
}

void ASuraPawnPlayer::HandleJumpInput(const FInputActionValue& Value)
{
	if (!MovementComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Player movement component is not valid!"));
		return;
	}
	
	bool bJumpPressed = Value.Get<bool>();
	
	MovementComponent->SetJumpPressed(bJumpPressed);
}


void ASuraPawnPlayer::HandleDashInput(const FInputActionValue& Value)
{
	if (!MovementComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Player movement component is not valid!"));
		return;
	}
	
	bool bDashPressed = Value.Get<bool>();

	MovementComponent->SetJumpPressed(bDashPressed);
}

void ASuraPawnPlayer::HandleCrouchInput(const FInputActionValue& Value)
{
	if (!MovementComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Player movement component is not valid!"));
		return;
	}
	
	bool bCrouchPressed = Value.Get<bool>();
	
	MovementComponent->SetJumpPressed(bCrouchPressed);
}




