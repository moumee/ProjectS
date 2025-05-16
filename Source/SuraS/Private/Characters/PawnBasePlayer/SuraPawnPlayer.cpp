// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PawnBasePlayer/SuraPawnPlayer.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "Characters/PawnBasePlayer/SuraPlayerMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"

#include "ActorComponents/WeaponSystem/WeaponSystemComponent.h"

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
	MovementComponent->SetDefaultCapsuleValues(CapsuleComponent->GetScaledCapsuleRadius(), CapsuleComponent->GetScaledCapsuleHalfHeight());
	
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// <WeaponSystem>
	WeaponSystem = CreateDefaultSubobject<UWeaponSystemComponent>(TEXT("WeaponSystem"));
	CapsuleComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
	ArmMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ASuraPawnPlayer::BeginPlay()
{
	Super::BeginPlay();

	
}

UCapsuleComponent* ASuraPawnPlayer::GetCapsuleComponent()
{
	return CapsuleComponent;
}

bool ASuraPawnPlayer::HasWeapon() const
{
	if (WeaponSystem)
	{
		return WeaponSystem->GetCurrentWeapon() != nullptr;
	}
	return false;
}

void ASuraPawnPlayer::UpdateLookInputVector2D(const FInputActionValue& InputValue)
{
	PlayerLookInputVector2D = InputValue.Get<FVector2D>();
}

void ASuraPawnPlayer::SetLookInputVector2DZero()
{
	PlayerLookInputVector2D = FVector2D::ZeroVector;
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
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ASuraPawnPlayer::StartJumpInput);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ASuraPawnPlayer::StopJumpInput);
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Started, this, &ASuraPawnPlayer::StartDashInput);
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Completed, this, &ASuraPawnPlayer::StopDashInput);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &ASuraPawnPlayer::StartCrouchInput);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &ASuraPawnPlayer::StopCrouchInput);

		// <WeaponSystem>
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASuraPawnPlayer::UpdateLookInputVector2D);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::None, this, &ASuraPawnPlayer::SetLookInputVector2DZero);
	}
}

void ASuraPawnPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
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

void ASuraPawnPlayer::StartJumpInput()
{
	if (!MovementComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Player movement component is not valid!"));
		return;
	}
	
	MovementComponent->SetJumpPressed(true);
}

void ASuraPawnPlayer::StopJumpInput()
{
	if (!MovementComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Player movement component is not valid!"));
		return;
	}
	
	MovementComponent->SetJumpPressed(false);
}



void ASuraPawnPlayer::StartDashInput()
{
	if (!MovementComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Player movement component is not valid!"));
		return;
	}

	MovementComponent->SetDashPressed(true);
}

void ASuraPawnPlayer::StopDashInput()
{
	if (!MovementComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Player movement component is not valid!"));
		return;
	}

	MovementComponent->SetDashPressed(false);
}

void ASuraPawnPlayer::StartCrouchInput()
{
	if (!MovementComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Player movement component is not valid!"));
		return;
	}
	
	MovementComponent->SetCrouchPressed(true);
}

void ASuraPawnPlayer::StopCrouchInput()
{
	if (!MovementComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Player movement component is not valid!"));
		return;
	}
	
	MovementComponent->SetCrouchPressed(false);
}






