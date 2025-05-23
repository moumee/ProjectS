// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/SuraCharacterPlayer.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ActorComponents/ACPlayerMovmentData.h"
#include "ActorComponents/DamageComponent/ACDamageSystem.h"
#include "Camera/CameraComponent.h"
#include "Characters/Player/SuraPlayerBaseState.h"
#include "Characters/Player/SuraPlayerCrouchingState.h"
#include "Characters/Player/SuraPlayerDashingState.h"
#include "Characters/Player/SuraPlayerFallingState.h"
#include "Characters/Player/SuraPlayerHangingState.h"
#include "Characters/Player/SuraPlayerJumpingState.h"
#include "Characters/Player/SuraPlayerMantlingState.h"
#include "Characters/Player/SuraPlayerRunningState.h"
#include "Characters/Player/SuraPlayerSlidingState.h"
#include "Characters/Player/SuraPlayerWalkingState.h"
#include "Characters/Player/SuraPlayerWallRunningState.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ActorComponents/UISystem/ACUIMangerComponent.h"

#include "ActorComponents/WeaponSystem/WeaponSystemComponent.h"

ASuraCharacterPlayer::ASuraCharacterPlayer()
{


	GetCharacterMovement()->bOrientRotationToMovement = false;

	PlayerMovementData = CreateDefaultSubobject<UACPlayerMovementData>("Player Movement Data");

	// Make character rotate with the controller
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetRootComponent(), FName(TEXT("Camera")));
	Camera->bUsePawnControlRotation = true;
	Camera->SetRelativeLocation(FVector(0.f, 0.f, 70.f));

	ArmMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Arm Mesh"));
	ArmMesh->SetupAttachment(Camera);

	DefaultCapsuleHalfHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	DefaultCameraLocation = Camera->GetRelativeLocation();

	// Initialize JumpsLeft
	JumpsLeft = MaxJumps;

	// UIManager actor components
	UIManager = CreateDefaultSubobject<UACUIMangerComponent>(TEXT("UI Manager Component"));
	
	

	DamageSystemComponent = CreateDefaultSubobject<UACDamageSystem>(TEXT("Damage System Component"));

	// Hit Effect Class Init - by Yoony
	static ConstructorHelpers::FClassFinder<UPlayerHitWidget> WidgetClass{ TEXT("/Game/UI/Player/WBP_PlayerHit") };

	if (WidgetClass.Succeeded())
		HitEffectWidgetClass = WidgetClass.Class;


	// WeaponSystem
	WeaponSystem = CreateDefaultSubobject<UWeaponSystemComponent>(TEXT("WeaponSystem"));
}

void ASuraCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	GetDamageSystemComponent()->OnDamaged.AddUObject(this, &ASuraCharacterPlayer::OnDamaged);
	GetDamageSystemComponent()->OnDeath.AddUObject(this, &ASuraCharacterPlayer::OnDeath);
	
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	DefaultGravityScale = PlayerMovementData->GetGravityScale();
	DefaultGroundFriction = GetCharacterMovement()->GroundFriction;
	DefaultBrakingFriction = GetCharacterMovement()->BrakingFriction;
	DefaultBrakingDecelerationFalling = GetCharacterMovement()->BrakingDecelerationFalling;
	DefaultBrakingDecelerationWalking = GetCharacterMovement()->BrakingDecelerationWalking;
	DefaultFallingLateralFriction = GetCharacterMovement()->FallingLateralFriction;

	DefaultCameraFOV = GetCamera()->FieldOfView;
	
	
	DefaultGroundFriction = GetCharacterMovement()->GroundFriction;

	MaxDashes = GetPlayerMovementData()->GetDashMaxStack();
	DashesLeft = MaxDashes;

	DashCooldowns.Init(0.f, MaxDashes);

	BaseMovementSpeed = GetPlayerMovementData()->GetRunSpeed();
	GetCharacterMovement()->AirControl = GetPlayerMovementData()->GetAirControl();
	GetCharacterMovement()->GravityScale = DefaultGravityScale;

	WalkingState = NewObject<USuraPlayerWalkingState>(this, USuraPlayerWalkingState::StaticClass());
	RunningState = NewObject<USuraPlayerRunningState>(this, USuraPlayerRunningState::StaticClass());
	JumpingState = NewObject<USuraPlayerJumpingState>(this, USuraPlayerJumpingState::StaticClass());
	FallingState = NewObject<USuraPlayerFallingState>(this, USuraPlayerFallingState::StaticClass());
	DashingState = NewObject<USuraPlayerDashingState>(this, USuraPlayerDashingState::StaticClass());
	CrouchingState = NewObject<USuraPlayerCrouchingState>(this, USuraPlayerCrouchingState::StaticClass());
	HangingState = NewObject<USuraPlayerHangingState>(this, USuraPlayerHangingState::StaticClass());
	MantlingState = NewObject<USuraPlayerMantlingState>(this, USuraPlayerMantlingState::StaticClass());
	WallRunningState = NewObject<USuraPlayerWallRunningState>(this, USuraPlayerWallRunningState::StaticClass());
	SlidingState = NewObject<USuraPlayerSlidingState>(this, USuraPlayerSlidingState::StaticClass());

	PreviousState = RunningState;
	PreviousGroundedState = RunningState;
	CurrentState = RunningState;

	// Hit Effect Widget Init - by Yoony
	if (IsValid(HitEffectWidgetClass))
	{
		HitEffectWidget = Cast<UPlayerHitWidget>(CreateWidget<UPlayerHitWidget>(GetWorld(), HitEffectWidgetClass));
		
		if (IsValid(HitEffectWidget))
		{
			HitEffectWidget->AddToViewport();
			HitEffectWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void ASuraCharacterPlayer::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);

}

void ASuraCharacterPlayer::ResetTriggeredBooleans()
{
	bJumpTriggered = false;
	bDashTriggered = false;
	bLandedTriggered = false;
}

void ASuraCharacterPlayer::SetBaseMovementSpeed(const float MovementSpeed)
{
	BaseMovementSpeed = MovementSpeed;
}

float ASuraCharacterPlayer::GetBaseMovementSpeed() const
{
	return BaseMovementSpeed;
}

void ASuraCharacterPlayer::ChangeState(USuraPlayerBaseState* NewState)
{
	if (!NewState) return;

	if (CurrentState)
	{
		CurrentState->ExitState(this);
		PreviousState = CurrentState;
	}
	CurrentState = NewState;
	CurrentState->EnterState(this);
}

bool ASuraCharacterPlayer::IsFallingDown() const
{
	
	return GetCharacterMovement()->IsFalling() && GetCharacterMovement()->Velocity.Z < 0.f;
}

bool ASuraCharacterPlayer::HasWeapon() const
{
	if (WeaponSystem)
	{
		return WeaponSystem->GetCurrentWeapon() != nullptr;
	}
	return false;
}

bool ASuraCharacterPlayer::HasMovementInput() const
{
	return ForwardAxisInputValue != 0.f || RightAxisInputValue != 0.f;
}

void ASuraCharacterPlayer::PrimaryJump()
{
	FVector JumpVector = FVector::UpVector * GetPlayerMovementData()->GetPrimaryJumpZSpeed();
	LaunchCharacter(JumpVector, false, true);
}

void ASuraCharacterPlayer::InAirJump()
{
	FVector InputDirection = GetActorForwardVector() * ForwardAxisInputValue + GetActorRightVector() * RightAxisInputValue;
	FVector LaunchVelocity = InputDirection * GetPlayerMovementData()->GetDoubleJumpXYSpeed() +
		FVector::UpVector * GetPlayerMovementData()->GetDoubleJumpZSpeed();
	LaunchCharacter(LaunchVelocity, false, true);
}

float ASuraCharacterPlayer::GetDefaultCapsuleHalfHeight() const
{
	return DefaultCapsuleHalfHeight;
}

FVector ASuraCharacterPlayer::GetDefaultCameraLocation() const
{
	return DefaultCameraLocation;
}

USuraPlayerBaseState* ASuraCharacterPlayer::GetCurrentState() const
{
	return CurrentState;
}

USuraPlayerBaseState* ASuraCharacterPlayer::GetPreviousState() const
{
	return PreviousState;
}

USuraPlayerBaseState* ASuraCharacterPlayer::GetPreviousGroundedState() const
{
	return PreviousGroundedState;
}

void ASuraCharacterPlayer::SetPreviousGroundedState(USuraPlayerBaseState* InState)
{
	PreviousGroundedState = InState;
}


void ASuraCharacterPlayer::PrintPlayerDebugInfo() const
{
	if (bIsDebugMode)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(106, 0.f, FColor::Green,
											 FString::Printf(TEXT("Current Player Speed : %f"), GetCharacterMovement()->Velocity.Size()));

			GEngine->AddOnScreenDebugMessage(105, 0.f, FColor::Green,
				FString::Printf(TEXT("Player Max Walk Speed : %f"), GetCharacterMovement()->MaxWalkSpeed));
			
			GEngine->AddOnScreenDebugMessage(104, 0.f, FColor::Green,
				FString::Printf(TEXT("Player Additional Speed : %f"), AdditionalMovementSpeed));

			GEngine->AddOnScreenDebugMessage(103, 0.f, FColor::Green,
				FString::Printf(TEXT("Player Base Speed : %f"), BaseMovementSpeed));

			GEngine->AddOnScreenDebugMessage(102, 0.f, FColor::Green,
				FString::Printf(TEXT("Jumps Left : %d / %d"), JumpsLeft, MaxJumps));

			GEngine->AddOnScreenDebugMessage(101, 0.f, FColor::Green,
				FString::Printf(TEXT("Dashes Left : %d / %d"), DashesLeft, MaxDashes));

			FString CooldownInfo;
			for (float Cooldown : DashCooldowns)
			{
				CooldownInfo += FString::Printf(TEXT("%.2f "), Cooldown);
			}

			GEngine->AddOnScreenDebugMessage(100, 0.f, FColor::Green,
				FString::Printf(TEXT("Dash Cooldowns: %s"), *CooldownInfo));

			GEngine->AddOnScreenDebugMessage(99, 0.f, FColor::Green,
				FString::Printf(TEXT("Input Axis Value : ( %f, %f )"), ForwardAxisInputValue, RightAxisInputValue));

			GEngine->AddOnScreenDebugMessage(97, 0.f, FColor::Red,
				FString::Printf(TEXT("Current State : %s"), *CurrentState->StateDisplayName.ToString()));

			GEngine->AddOnScreenDebugMessage(96, 0.f, FColor::Green,
				FString::Printf(TEXT("Previous Grounded State : %s"), *PreviousGroundedState->StateDisplayName.ToString()));

			GEngine->AddOnScreenDebugMessage(95, 0.f, FColor::Green,
				FString::Printf(TEXT("Previous State : %s"), *PreviousState->StateDisplayName.ToString()));

			GEngine->AddOnScreenDebugMessage(94, 0.f, FColor::Green,
				FString::Printf(TEXT("Wall Run Side : %s"), *UEnum::GetDisplayValueAsText(WallRunSide).ToString()));

			GEngine->AddOnScreenDebugMessage(93, 0.f, FColor::Green,
				FString::Printf(TEXT("Gravity Scale : %f"), GetCharacterMovement()->GravityScale));
		}
	}
}

void ASuraCharacterPlayer::UpdateDashCooldowns(float DeltaTime)
{
	if (DashesLeft == MaxDashes) return;

	float MinCurrentCooldown = FLT_MAX;
	int32 CurrentIndex = -1;

	for (int32 i = 0; i < MaxDashes; i++)
	{
		if (DashCooldowns[i] > 0.f)
		{
			if (DashCooldowns[i] < MinCurrentCooldown)
			{
				MinCurrentCooldown = DashCooldowns[i];
				CurrentIndex = i;
			}
		}
	}
	
	if (CurrentIndex != -1)
	{
		DashCooldowns[CurrentIndex] = DashCooldowns[CurrentIndex] - DeltaTime;
		if (DashCooldowns[CurrentIndex] <= KINDA_SMALL_NUMBER)
		{
			DashCooldowns[CurrentIndex] = 0.f;
			DashesLeft = FMath::Min(DashesLeft + 1, MaxDashes);
		}
	}
	
	
}

void ASuraCharacterPlayer::OnDamaged()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Player Damaged"));

	HitEffectWidget->SetVisibility(ESlateVisibility::Visible);
	HitEffectWidget->PlayFadeAnimtion();
}

void ASuraCharacterPlayer::OnDeath()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Player Dead"));
}

float ASuraCharacterPlayer::FindFloorAngle() const
{
	if (!GetCharacterMovement()->IsFalling() && GetCharacterMovement()->MovementMode != MOVE_Flying)
	{
		const FVector FloorNormal = GetCharacterMovement()->CurrentFloor.HitResult.Normal;
		const float FloorAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(FloorNormal, FVector::UpVector)));
		const FVector DirectionXY = FVector(GetVelocity().X, GetVelocity().Y, 0.f).GetSafeNormal();
		return FVector::DotProduct(FloorNormal, DirectionXY) > 0.f ? -FloorAngle : FloorAngle;
	}
	return 0.f;
	
}

void ASuraCharacterPlayer::RestoreCameraTilt(float DeltaTime)
{
	if (bShouldRestoreCameraTilt)
	{
		FRotator CurrentControlRotation = GetControlRotation();
		FRotator NewRotation = FMath::RInterpTo(CurrentControlRotation,
		                                        FRotator(CurrentControlRotation.Pitch, CurrentControlRotation.Yaw, 0.f), DeltaTime, 10.f);
		GetController()->SetControlRotation(NewRotation);
		if (FMath::Abs(GetControlRotation().Roll) < 0.1f)
		{
			FRotator CurrentRotation = GetControlRotation();
			GetController()->SetControlRotation(FRotator(CurrentRotation.Pitch, CurrentRotation.Yaw, 0.f));
			bShouldRestoreCameraTilt = false;
		}
	}
}


void ASuraCharacterPlayer::InterpCameraFOV(float DeltaTime)
{
	float Alpha = FMath::Clamp(GetCharacterMovement()->Velocity.Size() / PlayerMovementData->GetMaxCameraFOVSpeed(), 0.f, 1.f);
	float TargetFOV = FMath::Lerp(DefaultCameraFOV, PlayerMovementData->GetMaxCameraFOV(), Alpha);
	float NewCameraFOV = FMath::FInterpTo(GetCamera()->FieldOfView, TargetFOV, DeltaTime, 10.f);
	GetCamera()->SetFieldOfView(NewCameraFOV);
}

void ASuraCharacterPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	XYSpeed = FVector(GetCharacterMovement()->Velocity.X, GetCharacterMovement()->Velocity.Y, 0.f).Size();

	InterpCameraFOV(DeltaTime);

	UpdateDashCooldowns(DeltaTime);

	RestoreCameraTilt(DeltaTime);

	PrintPlayerDebugInfo();

	if (CurrentState)
	{
		CurrentState->UpdateState(this, DeltaTime);
	}
	
	GetCharacterMovement()->MaxWalkSpeed = BaseMovementSpeed;
}

void ASuraCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASuraCharacterPlayer::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &ASuraCharacterPlayer::StopMoving);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASuraCharacterPlayer::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ASuraCharacterPlayer::StartJumping);
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Triggered, this, &ASuraCharacterPlayer::StartDashing);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &ASuraCharacterPlayer::StartCrouching);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &ASuraCharacterPlayer::StopCrouching);
		
	}
}

void ASuraCharacterPlayer::Move(const FInputActionValue& InputValue)
{
	FVector2D InputVector = InputValue.Get<FVector2D>();
	
	ForwardAxisInputValue = InputVector.Y;
	RightAxisInputValue = InputVector.X;

	CurrentState->Move(this, InputVector);

}

void ASuraCharacterPlayer::StopMoving()
{
	ForwardAxisInputValue = 0.f;
	RightAxisInputValue = 0.f;

	CurrentState->StopMoving(this);
	
}

void ASuraCharacterPlayer::Look(const FInputActionValue& InputValue)
{
	FVector2D InputVector = InputValue.Get<FVector2D>();

	CurrentState->Look(this, InputVector);
}

void ASuraCharacterPlayer::StartJumping()
{
	bJumpTriggered = true;
}

void ASuraCharacterPlayer::StartCrouching()
{
	bCrouchTriggered = true;
}

void ASuraCharacterPlayer::StopCrouching()
{
	bCrouchTriggered = false;
}

void ASuraCharacterPlayer::StartDashing()
{
	bDashTriggered = true;
}

void ASuraCharacterPlayer::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	bLandedTriggered = true;
	JumpsLeft = MaxJumps;
	CurrentState->Landed(this, Hit);
}

bool ASuraCharacterPlayer::ShouldEnterWallRunning(FVector& OutWallRunDirection, EWallSide& OutWallRunSide)
{
	if (!GetCharacterMovement()->IsFalling()) return false;

	if (bCrouchTriggered) return false;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	FHitResult LeftHit;
	bool bLeftHit = GetWorld()->LineTraceSingleByChannel(LeftHit, GetActorLocation(),
		GetActorLocation() + GetActorRightVector() * -55.f, ECC_GameTraceChannel2, Params);
	bool bLeftWallRunnable = false;
	FHitResult RightHit;
	bool bRightHit = GetWorld()->LineTraceSingleByChannel(RightHit, GetActorLocation(),
		GetActorLocation() + GetActorRightVector() * 55.f, ECC_GameTraceChannel2, Params);
	bool bRightWallRunnable = false;

	if (bLeftHit)
	{
		if (LeftHit.ImpactNormal.Z > -0.05f && LeftHit.ImpactNormal.Z < GetCharacterMovement()->GetWalkableFloorZ())
		{
			bLeftWallRunnable = true;
		}
	}

	if (bRightHit)
	{
		if (RightHit.ImpactNormal.Z > -0.05f && RightHit.ImpactNormal.Z < GetCharacterMovement()->GetWalkableFloorZ())
		{
			bRightWallRunnable = true;
		}
	}

	if (bLeftWallRunnable && bRightWallRunnable)
	{
		if (LeftHit.Distance < RightHit.Distance)
		{
			OutWallRunSide = EWallSide::Left;
			FVector WallNormalXY = FVector(LeftHit.ImpactNormal.X, LeftHit.ImpactNormal.Y, 0.f);
			OutWallRunDirection = FVector::CrossProduct(WallNormalXY, FVector::UpVector).GetSafeNormal();
		}
		else
		{
			OutWallRunSide = EWallSide::Right;
			FVector WallNormalXY = FVector(RightHit.ImpactNormal.X, RightHit.ImpactNormal.Y, 0.f);
			OutWallRunDirection = FVector::CrossProduct(WallNormalXY, FVector::DownVector).GetSafeNormal();
		}
		return true;
	}

	if (bLeftWallRunnable)
	{
		OutWallRunSide = EWallSide::Left;
		FVector WallNormalXY = FVector(LeftHit.ImpactNormal.X, LeftHit.ImpactNormal.Y, 0.f);
		OutWallRunDirection = FVector::CrossProduct(WallNormalXY, FVector::UpVector).GetSafeNormal();
		return true;
	}

	if (bRightWallRunnable)
	{
		OutWallRunSide = EWallSide::Right;
		FVector WallNormalXY = FVector(RightHit.ImpactNormal.X, RightHit.ImpactNormal.Y, 0.f);
		OutWallRunDirection = FVector::CrossProduct(WallNormalXY, FVector::DownVector).GetSafeNormal();
		return true;
	}

	return false;

	
}

void ASuraCharacterPlayer::InterpCapsuleHeight(float TargetScale, float DeltaTime)
{
	float CurrentHeight = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	float TargetHeight = GetDefaultCapsuleHalfHeight() * TargetScale;
	float NewHeight = FMath::FInterpTo(CurrentHeight, TargetHeight, DeltaTime, 10.f);
    
	// Calculate how much the height will change
	float HeightDifference = NewHeight - CurrentHeight;
    
	// Move the actor UP by HALF the difference to keep the bottom fixed
	// (We need half because the capsule's origin is at its center)
	FVector AdjustmentVector(0, 0, HeightDifference * 0.5f);
	AddActorWorldOffset(AdjustmentVector);
    
	// Now change the capsule height
	GetCapsuleComponent()->SetCapsuleHalfHeight(NewHeight);
	

}

bool ASuraCharacterPlayer::TakeDamage(const FDamageData& DamageData, const AActor* DamageCauser)
{
	return GetDamageSystemComponent()->TakeDamage(DamageData, DamageCauser);
}

void ASuraCharacterPlayer::StartCamShake(const TSubclassOf<UCameraShakeBase> InShakeClass)
{
	if (!InShakeClass) return;
	
	if (APlayerController* PlayerController = GetController<APlayerController>())
	{
		PlayerController->PlayerCameraManager->StartCameraShake(InShakeClass);
	}
}




