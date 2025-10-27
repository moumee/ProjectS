// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PawnBasePlayer/SuraPawnPlayer.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "NiagaraComponent.h"
#include "ActorComponents/AttackComponents/ACPlayerAttackTokens.h"
#include "ActorComponents/DamageComponent/ACDamageSystem.h"
#include "ActorComponents/UISystem/ACHitScreenManager.h"
#include "ActorComponents/UISystem/ACPlayerHealthComponent.h"
#include "ActorComponents/UISystem/ACUIMangerComponent.h"
#include "Camera/CameraComponent.h"
#include "Characters/PawnBasePlayer/SuraPlayerCameraComponent.h"
#include "Characters/PawnBasePlayer/SuraPlayerMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SceneCaptureComponent2D.h" //<JaeHyeong>

#include "ActorComponents/WeaponSystem/WeaponSystemComponent.h"
#include "Characters/Enemies/SuraCharacterEnemyBase.h"
#include "Characters/PawnBasePlayer/PlayerSound_DataAsset.h"
#include "GameModes/SuraLevelGameMode.h"
#include "Instance/SuraCheckpointSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGame/SuraSaveGame.h"
#include "Slate/SGameLayerManager.h"

ASuraPawnPlayer::ASuraPawnPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Component"));
	RootComponent = CapsuleComponent;
	CapsuleComponent->SetSimulatePhysics(false);
	CapsuleComponent->InitCapsuleSize(40.f, 90.f);
	CapsuleComponent->SetCollisionProfileName(TEXT("Pawn"));
	CapsuleComponent->SetNotifyRigidBodyCollision(true);
	

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CapsuleComponent);
	Camera->SetRelativeLocation(FVector(0.f, 0.f, 70.f));
	Camera->bUsePawnControlRotation = true;
	Camera->PostProcessSettings.bOverride_DepthOfFieldFocalDistance = true;

	ArmMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Arm Mesh"));
	ArmMesh->SetupAttachment(Camera);

	HandsMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hands Mesh"));
	HandsMesh->SetupAttachment(ArmMesh);
	HandsMesh->SetLeaderPoseComponent(ArmMesh);

	MovementComponent = CreateDefaultSubobject<USuraPlayerMovementComponent>(TEXT("Movement Component"));
	MovementComponent->UpdatedComponent = RootComponent;
	MovementComponent->SetDefaultCapsuleValues(CapsuleComponent->GetScaledCapsuleRadius(), CapsuleComponent->GetScaledCapsuleHalfHeight());

	CameraMovementComponent = CreateDefaultSubobject<USuraPlayerCameraComponent>(TEXT("CameraMovement Component"));
	
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// <WeaponSystem>
	WeaponSystem = CreateDefaultSubobject<UWeaponSystemComponent>(TEXT("WeaponSystem"));
	CapsuleComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel7, ECR_Ignore);
	CapsuleComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel8, ECR_Ignore);
	ArmMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// FPSceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent")); //<JaeHyeong>
	// FPSceneCapture->SetupAttachment(Camera);

	// for damage interactions with enemies
	AttackTokensComponent = CreateDefaultSubobject<UACPlayerAttackTokens>(TEXT("Attack Tokens Component"));
	DamageSystemComponent = CreateDefaultSubobject<UACDamageSystem>(TEXT("Damage System Component"));

	// UIManager actor components - suhyeon
	UIManager = CreateDefaultSubobject<UACUIMangerComponent>(TEXT("UI Manager Component"));
	HealthComponent = CreateDefaultSubobject<UACPlayerHealthComponent>(TEXT("Health Component"));
	HitScreenManager = CreateDefaultSubobject<UACHitScreenManager>(TEXT("HitScreen Manager Component"));

	ForwardDashEffectComponent = CreateDefaultSubobject<UNiagaraComponent>("Forward Dash Effect Component");
	ForwardDashEffectComponent->SetupAttachment(Camera);
	ForwardDashEffectComponent->SetAutoActivate(false);

	BackwardDashEffectComponent = CreateDefaultSubobject<UNiagaraComponent>("Backward Dash Effect Component");
	BackwardDashEffectComponent->SetupAttachment(Camera);
	BackwardDashEffectComponent->SetAutoActivate(false);

	LeftDashEffectComponent = CreateDefaultSubobject<UNiagaraComponent>("Left Dash Effect Component");
	LeftDashEffectComponent->SetupAttachment(Camera);
	LeftDashEffectComponent->SetAutoActivate(false);

	RightDashEffectComponent = CreateDefaultSubobject<UNiagaraComponent>("Right Dash Effect Component");
	RightDashEffectComponent->SetupAttachment(Camera);
	RightDashEffectComponent->SetAutoActivate(false);
}

void ASuraPawnPlayer::BeginPlay()
{
	Super::BeginPlay();

	if (USuraCheckpointSubsystem* CheckpointSubsystem = GetGameInstance()->GetSubsystem<USuraCheckpointSubsystem>())
	{
		CheckpointSubsystem->OnCheckpointLoadedDelegate.AddDynamic(this, &ThisClass::OnCheckPointLoaded);
	}

	
	GetDamageSystemComponent()->OnDamaged.AddUObject(CameraMovementComponent, &USuraPlayerCameraComponent::OnDamaged);
	GetDamageSystemComponent()->OnDamaged.AddUObject(this, &ASuraPawnPlayer::OnDamaged);
	GetDamageSystemComponent()->OnDeath.AddUObject(this, &ASuraPawnPlayer::OnDeath);

	GetPlayerMovementComponent()->OnDash.AddUObject(this, &ASuraPawnPlayer::OnDash);
	GetPlayerMovementComponent()->OnDashEnd.AddUObject(this, &ASuraPawnPlayer::OnDashEnd);

	FTimerDelegate PlayerHealthCheckTimerDelegate;
	PlayerHealthCheckTimerDelegate.BindUObject(this, &ASuraPawnPlayer::CheckPlayerHealth);
	GetWorld()->GetTimerManager().SetTimer(PlayerHealthCheckTimer, PlayerHealthCheckTimerDelegate,
		CorrectionSystemCheckTime, true);

	DefaultCameraRelativeLocation = Camera->GetRelativeLocation();
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
		EnhancedInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &ASuraPawnPlayer::StartShiftInput);
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

void ASuraPawnPlayer::CheckPlayerHealth()
{
	// UE_LOG(LogTemp, Display, TEXT("Checked Player Health. Current Player Health : %f"), DamageSystemComponent->GetHealth());
	if (DamageSystemComponent->GetHealth() <= ConditionalPlayerHP)
	{
		OnPlayerHealthHalved.Broadcast();
		// Reduce the number of max tokens in the AttackTokensComponent
		AttackTokensComponent->SetMaxEnemyAttackTokens(3); // TODO: make magic numbers into variables in a data table
		AttackTokensComponent->SetMaxEnemyPursuitTokens(3);
	}
	else
	{
		// Restore the number of max tokens
		AttackTokensComponent->SetMaxEnemyAttackTokens(8);
		AttackTokensComponent->SetMaxEnemyPursuitTokens(6);
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
	if (MovementComponent->GetMovementState() == EMovementState::EMS_Downed ||
		MovementComponent->GetMovementState() == EMovementState::EMS_Dead)
	{
		return;
	}
	
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


void ASuraPawnPlayer::StartShiftInput()
{
	if (!MovementComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Player movement component is not valid!"));
		return;
	}
	
	MovementComponent->SetShiftPressed(true);
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

bool ASuraPawnPlayer::TakeDamage(const FDamageData& DamageData, AActor* DamageCauser)
{
	if (MovementComponent->GetIsInvincible())
	{
		return false;
	}
	
	// UIManager 컴포넌트에서 DamageIndicator 위젯을 가져옵니다.
	if (UIManager)
	{
		UIManager->ShowDamageIndicator(DamageCauser);
	}

	GetPlayerMovementComponent()->NotifyDamageData(DamageData.DamageType, DamageData.ImpulseDirection, DamageData.ImpulseMagnitude);
	
	switch (DamageData.DamageType)
	{
		case EDamageType::Charge:
			{
				if (ensureAlways(PlayerSound_DataAsset))
				{
					USoundBase* HitSound = PlayerSound_DataAsset->DownedHitSound;
					if (!HitSound) break;
					UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
					break;
				}
			}
		default:
			{
				if (ensureAlways(PlayerSound_DataAsset))
				{
					USoundBase* HitSound = PlayerSound_DataAsset->NormalHitSound;
					if (!HitSound) break;
					UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
					break;
				}
			}
	}
	
	return GetDamageSystemComponent()->TakeDamage(DamageData, DamageCauser);
}

void ASuraPawnPlayer::GravityLaunchPlayer(const FVector& Direction, float ForceAmount)
{
	GetPlayerMovementComponent()->NotifyGravityLaunchForce(Direction, ForceAmount);
}

void ASuraPawnPlayer::JumpPadLaunchPlayer(float ForceAmount)
{
	GetPlayerMovementComponent()->NotifyJumpPadLaunchForce(ForceAmount);
}

void ASuraPawnPlayer::RequestMovementDataModification(const TArray<FPlayerMovementDataModifier>& Modifiers)
{
	GetPlayerMovementComponent()->NotifyMovementDataModification(Modifiers);
}

void ASuraPawnPlayer::RequestMovementKeyHoldModification(const TArray<FPlayerKeyHoldModifier>& Modifiers)
{
	GetPlayerMovementComponent()->NotifyMovementKeyHoldModification(Modifiers);
}

void ASuraPawnPlayer::RequestResetModification()
{
	GetPlayerMovementComponent()->NotifyResetModification();
}

void ASuraPawnPlayer::OnDamaged()
{
	if (HealthComponent) 
	{
		HealthComponent->TakeDamage(10.0f); // default damage 10으로 일단 고정. 추후 변수로 변경
	}
}

void ASuraPawnPlayer::OnDeath()
{
	GEngine->AddOnScreenDebugMessage(10, 15.0f, FColor::Yellow, TEXT("Player Dead"));
	if (ASuraLevelGameMode* GameMode = Cast<ASuraLevelGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		GameMode->OnPlayerDeath(this);
	}
}

void ASuraPawnPlayer::OnDash(FVector2D MovementInput)
{
	if (MovementInput.IsZero())
	{
		ForwardDashEffectComponent->Activate();
	}
	else
	{
		if (MovementInput.Y > 0)
		{
			ForwardDashEffectComponent->Activate();
		}
		else if (MovementInput.Y < 0)
		{
			BackwardDashEffectComponent->Activate();
		}
		else
		{
			if (MovementInput.X < 0)
			{
				LeftDashEffectComponent->Activate();
			}
			else if (MovementInput.X > 0)
			{
				RightDashEffectComponent->Activate();
			}
		}
	}
	
}

void ASuraPawnPlayer::OnDashEnd()
{
	ForwardDashEffectComponent->Deactivate();
	BackwardDashEffectComponent->Deactivate();
	LeftDashEffectComponent->Deactivate();
	RightDashEffectComponent->Deactivate();
}

void ASuraPawnPlayer::OnCheckPointLoaded()
{
	if (USuraCheckpointSubsystem* Subsystem = GetGameInstance()->GetSubsystem<USuraCheckpointSubsystem>())
	{
		USuraSaveGame* SaveData = Subsystem->GetCurrentSave();
		check(SaveData);

		FName CurrentMapName = FName(*UGameplayStatics::GetCurrentLevelName(this, true));
		if (SaveData->MapName == CurrentMapName)
		{
			TeleportTo(SaveData->SpawnTransform.GetLocation(), SaveData->SpawnTransform.Rotator());
		}
		else
		{
			UGameplayStatics::OpenLevel(this, SaveData->MapName);
		}
	}
}







