// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PawnBasePlayer/SuraPawnPlayer.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "NiagaraComponent.h"
#include "ActorComponents/AttackComponents/ACPlayerAttackTokens.h"
#include "ActorComponents/DamageComponent/ACDamageSystem.h"
#include "ActorComponents/UISystem/ACHitScreenManager.h"
#include "ActorComponents/UISystem/ACUIMangerComponent.h"
#include "Camera/CameraComponent.h"
#include "Characters/PawnBasePlayer/SuraPlayerCameraComponent.h"
#include "Characters/PawnBasePlayer/SuraPlayerMovementComponent.h"
#include "Components/CapsuleComponent.h"

#include "ActorComponents/WeaponSystem/WeaponSystemComponent.h"
#include "Characters/Enemies/SuraCharacterEnemyBase.h"
#include "Characters/Enemies/Boss/SuraCharacterBossProto.h"
#include "Characters/PawnBasePlayer/PlayerSound_DataAsset.h"
#include "Components/AudioComponent.h"
#include "GameModes/SuraLevelGameMode.h"
#include "Instance/SuraCheckpointSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGame/SuraSaveGame.h"
#include "UI/CustomGameInstance.h"

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
	DefaultCameraRelativeLocation = Camera->GetRelativeLocation();

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

	WallRunAudioComponent = CreateDefaultSubobject<UAudioComponent>("WallRunAudioComponent");
	WallRunAudioComponent->SetupAttachment(RootComponent);
	WallRunAudioComponent->bAutoActivate = false;

	SlideAudioComponent = CreateDefaultSubobject<UAudioComponent>("SlideAudioComponent");
	SlideAudioComponent->SetupAttachment(RootComponent);
	SlideAudioComponent->bAutoActivate = false;
}

void ASuraPawnPlayer::BeginPlay()
{
	Super::BeginPlay();

	Camera->SetRelativeLocation(DefaultCameraRelativeLocation);

	// Crash the game if there is no data asset assigned
	checkf(PlayerSound_DataAsset, TEXT("Player sound data asset is not assigned"));

	WallRunAudioComponent->SetSound(PlayerSound_DataAsset->WallRunSound.Sound);
	WallRunAudioComponent->OnAudioPlaybackPercent.AddDynamic(this, &ASuraPawnPlayer::HandleWallRunAudioPlayback);
	SlideAudioComponent->SetSound(PlayerSound_DataAsset->SlideSound.Sound);
	SlideAudioComponent->OnAudioPlaybackPercent.AddDynamic(this, &ThisClass::HandleSlideAudioPlayback);

	
	GetDamageSystemComponent()->OnDamaged.AddUObject(CameraMovementComponent, &USuraPlayerCameraComponent::OnDamaged);
	GetDamageSystemComponent()->OnDeath.AddUObject(this, &ASuraPawnPlayer::OnDeath);

	GetPlayerMovementComponent()->OnPrimaryJumpDelegate.AddDynamic(this, &ASuraPawnPlayer::OnPrimaryJump);
	GetPlayerMovementComponent()->OnDoubleJumpDelegate.AddDynamic(this, &ASuraPawnPlayer::OnDoubleJump);
	GetPlayerMovementComponent()->OnWallJumpDelegate.AddDynamic(this, &ASuraPawnPlayer::OnWallJump);
	GetPlayerMovementComponent()->OnWallRunDelegate.AddDynamic(this, &ASuraPawnPlayer::OnWallRun);
	GetPlayerMovementComponent()->OnWallRunEndDelegate.AddDynamic(this, &ASuraPawnPlayer::OnWallRunEnd);
	GetPlayerMovementComponent()->OnSlideDelegate.AddDynamic(this, &ASuraPawnPlayer::OnSlide);
	GetPlayerMovementComponent()->OnSlideEndDelegate.AddDynamic(this, &ASuraPawnPlayer::OnSlideEnd);
	GetPlayerMovementComponent()->OnLandDelegate.AddDynamic(this, &ASuraPawnPlayer::OnLand);
	GetPlayerMovementComponent()->OnDashDelegate.AddDynamic(this, &ASuraPawnPlayer::OnDash);
	GetPlayerMovementComponent()->OnDashEndDelegate.AddDynamic(this, &ASuraPawnPlayer::OnDashEnd);
	

	CachedGameInstance = Cast<UCustomGameInstance>(GetGameInstance());
	if (!CachedGameInstance)
	{
		UE_LOG(LogTemp, Error, TEXT("ASuraPawnPlayer::BeginPlay - CachedGameInstance is invalid!!"));
	}

	if (ASuraLevelGameMode* GameMode = Cast<ASuraLevelGameMode>(GetWorld()->GetAuthGameMode()))
	{
		GameMode->TeleportToLastCheckpoint();
	}
}

void ASuraPawnPlayer::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetDamageSystemComponent()->OnDeath.RemoveAll(this);
	
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

		// Developer Action
		EnhancedInputComponent->BindAction(TeleportToLastCheckpointAction, ETriggerEvent::Started, this, &ThisClass::StartTeleportToLastCheckpointInput);

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

void ASuraPawnPlayer::OnPrimaryJump()
{
	const FPlayerSoundData& Data = PlayerSound_DataAsset->PrimaryJumpSound;
	float VolumeMultiplier, PitchMultiplier;

	float Speed = GetPlayerMovementComponent()->Velocity.Size();
	
	CalculateMappedSoundValue(Data, Speed, VolumeMultiplier, PitchMultiplier);

	UGameplayStatics::SpawnSoundAttached(Data.Sound, GetRootComponent(), NAME_None,
		FVector(ForceInit), FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset,
		false, VolumeMultiplier, PitchMultiplier);

	if (Data.bDebug)
	{
		GEngine->AddOnScreenDebugMessage(1256, Data.DebugDisplayDuration, FColor::Green,
			FString::Printf(TEXT("Primary Jump Sound Evaluation Speed: %.0f, "
						"Mapped Volume Multiplier: %.3f, Mapped Pitch Multiplier: %.3f"), Speed, VolumeMultiplier, PitchMultiplier));
	}
}

void ASuraPawnPlayer::OnDoubleJump()
{
	const FPlayerSoundData& Data = PlayerSound_DataAsset->DoubleJumpSound;
	float VolumeMultiplier, PitchMultiplier;
	float Speed = GetPlayerMovementComponent()->Velocity.Size();
	CalculateMappedSoundValue(Data, GetPlayerMovementComponent()->Velocity.Size(), VolumeMultiplier, PitchMultiplier);
	
	UGameplayStatics::SpawnSoundAttached(PlayerSound_DataAsset->DoubleJumpSound.Sound, GetRootComponent(), NAME_None,
		FVector(ForceInit), FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset,
		false, VolumeMultiplier, PitchMultiplier);

	if (Data.bDebug)
	{
		if (!GEngine) return;
		GEngine->AddOnScreenDebugMessage(1257, Data.DebugDisplayDuration, FColor::Green,
			FString::Printf(TEXT("Double Jump Sound Evaluation Speed: %.0f, "
						"Mapped Volume Multiplier: %.3f, Mapped Pitch Multiplier: %.3f"), Speed, VolumeMultiplier, PitchMultiplier));
	}
}

void ASuraPawnPlayer::OnWallJump()
{
	const FPlayerSoundData& Data = PlayerSound_DataAsset->PrimaryJumpSound;
	float VolumeMultiplier, PitchMultiplier;

	float Speed = GetPlayerMovementComponent()->Velocity.Size();
	
	CalculateMappedSoundValue(Data, Speed, VolumeMultiplier, PitchMultiplier);

	UGameplayStatics::SpawnSoundAttached(Data.Sound, GetRootComponent(), NAME_None,
		FVector(ForceInit), FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset,
		false, VolumeMultiplier, PitchMultiplier);

	if (Data.bDebug)
	{
		if (!GEngine) return;
		GEngine->AddOnScreenDebugMessage(1258, Data.DebugDisplayDuration, FColor::Green,
			FString::Printf(TEXT("Primary Jump Sound Evaluation Speed: %.0f, "
						"Mapped Volume Multiplier: %.3f, Mapped Pitch Multiplier: %.3f"), Speed, VolumeMultiplier, PitchMultiplier));
	}
}

void ASuraPawnPlayer::OnSlide()
{
	const FPlayerSoundData& Data = PlayerSound_DataAsset->SlideSound;
	float VolumeMultiplier, PitchMultiplier;
	float Speed = GetPlayerMovementComponent()->Velocity.Size();
	CalculateMappedSoundValue(Data, Speed, VolumeMultiplier, PitchMultiplier);

	SlideAudioComponent->VolumeMultiplier = VolumeMultiplier;
	SlideAudioComponent->PitchMultiplier = PitchMultiplier;

	if (Data.bDebug)
	{
		if (!GEngine) return;
		GEngine->AddOnScreenDebugMessage(1259, Data.DebugDisplayDuration, FColor::Green,
			FString::Printf(TEXT("Slide Sound Evaluation Speed: %.0f, "
				"Mapped Volume Multiplier: %.3f, Mapped Pitch Multiplier: %.3f"), Speed, VolumeMultiplier, PitchMultiplier));
	}

	float TimeSinceSlideEnd = GetWorld()->GetTimeSeconds() - SlideEndTime;
	if (TimeSinceSlideEnd > 0.75f)
	{
		SlideAudioComponent->SetBoolParameter("SkipStart", false);
		SlideAudioComponent->Play();
	}
	else
	{
		SlideAudioComponent->SetBoolParameter("SkipStart", true);
		SlideAudioComponent->Play();
	}
	
}

void ASuraPawnPlayer::OnSlideEnd()
{
	SlideAudioComponent->FadeOut(0.4f, 0.f);
	SlideEndTime = GetWorld()->GetTimeSeconds();
}

void ASuraPawnPlayer::OnWallRun()
{
	const FPlayerSoundData& Data = PlayerSound_DataAsset->WallRunSound;
	float VolumeMultiplier, PitchMultiplier;
	float Speed = GetPlayerMovementComponent()->Velocity.Size();
	CalculateMappedSoundValue(Data, Speed, VolumeMultiplier, PitchMultiplier);

	WallRunAudioComponent->VolumeMultiplier = VolumeMultiplier;
	WallRunAudioComponent->PitchMultiplier = PitchMultiplier;

	if (Data.bDebug)
	{
		if (!GEngine) return;
		GEngine->AddOnScreenDebugMessage(1260, Data.DebugDisplayDuration, FColor::Green,
			FString::Printf(TEXT("Wall Run Sound Evaluation Speed: %.0f, "
				"Mapped Volume Multiplier: %.3f, Mapped Pitch Multiplier: %.3f"), Speed, VolumeMultiplier, PitchMultiplier));
	}
	
	WallRunAudioComponent->Play();

	
}

void ASuraPawnPlayer::OnWallRunEnd()
{
	WallRunAudioComponent->FadeOut(0.4f, 0.f);
}

void ASuraPawnPlayer::OnLand(float ZSpeed)
{
	const FPlayerSoundData& Data = PlayerSound_DataAsset->LandSound;
	float VolumeMultiplier, PitchMultiplier;

	float Speed = FMath::Abs(ZSpeed);
	
	CalculateMappedSoundValue(Data, Speed, VolumeMultiplier, PitchMultiplier);

	UGameplayStatics::SpawnSoundAttached(Data.Sound, GetRootComponent(), NAME_None,
		FVector(ForceInit), FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset,
		false, VolumeMultiplier, PitchMultiplier);

	if (Data.bDebug)
	{
		if (!GEngine) return;
		GEngine->AddOnScreenDebugMessage(1261, Data.DebugDisplayDuration, FColor::Green,
			FString::Printf(TEXT("Land Sound Evaluation Speed: %.0f, "
						"Mapped Volume Multiplier: %.3f, Mapped Pitch Multiplier: %.3f"), Speed, VolumeMultiplier, PitchMultiplier));
	}
}

void ASuraPawnPlayer::HandleWallRunAudioPlayback(const USoundWave* PlayingSoundWave, const float PlaybackPercent)
{
	const FPlayerSoundData& Data = PlayerSound_DataAsset->WallRunSound;
	float VolumeMultiplier, PitchMultiplier;
	float Speed = GetPlayerMovementComponent()->Velocity.Size();
	CalculateMappedSoundValue(Data, Speed, VolumeMultiplier, PitchMultiplier);

	WallRunAudioComponent->VolumeMultiplier = VolumeMultiplier;
	WallRunAudioComponent->PitchMultiplier = PitchMultiplier;

	if (Data.bDebug)
	{
		if (!GEngine) return;
		GEngine->AddOnScreenDebugMessage(1262, Data.DebugDisplayDuration, FColor::Green,
			FString::Printf(TEXT("Wall Run Sound Evaluation Speed: %.0f, "
				"Mapped Volume Multiplier: %.3f, Mapped Pitch Multiplier: %.3f"), Speed, VolumeMultiplier, PitchMultiplier));
	}
}

void ASuraPawnPlayer::HandleSlideAudioPlayback(const USoundWave* PlayingSoundWave, const float PlaybackPercent)
{
	const FPlayerSoundData& Data = PlayerSound_DataAsset->SlideSound;
	float VolumeMultiplier, PitchMultiplier;
	float Speed = GetPlayerMovementComponent()->Velocity.Size();
	CalculateMappedSoundValue(Data, Speed, VolumeMultiplier, PitchMultiplier);

	SlideAudioComponent->VolumeMultiplier = VolumeMultiplier;
	SlideAudioComponent->PitchMultiplier = PitchMultiplier;

	if (Data.bDebug)
	{
		if (!GEngine) return;
		GEngine->AddOnScreenDebugMessage(1263, Data.DebugDisplayDuration, FColor::Green,
			FString::Printf(TEXT("Slide Sound Evaluation Speed: %.0f, "
				"Mapped Volume Multiplier: %.3f, Mapped Pitch Multiplier: %.3f"), Speed, VolumeMultiplier, PitchMultiplier));
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

	if (CachedGameInstance) 
	{
		AddControllerYawInput(InputVector.X * CachedGameInstance->MouseSensitivity);
		AddControllerPitchInput(InputVector.Y * CachedGameInstance->MouseSensitivity);
	}
	else
	{
		AddControllerYawInput(InputVector.X * 1.0f);
		AddControllerPitchInput(InputVector.Y * 1.0f);
	}
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

void ASuraPawnPlayer::StartTeleportToLastCheckpointInput()
{
	if (ASuraLevelGameMode* LevelGameMode = Cast<ASuraLevelGameMode>(GetWorld()->GetAuthGameMode()))
	{
		LevelGameMode->TeleportToLastCheckpoint();
	}
}

void ASuraPawnPlayer::CalculateMappedSoundValue(const FPlayerSoundData& Data, float Speed,
                                                float& OutVolumeMultiplier, float& OutPitchMultiplier)
{
	if (Data.bMapVolume)
	{
		TRange<float> VolumeSpeedRange = TRange<float>(Data.VolumeSpeedRange.Min, Data.VolumeSpeedRange.Max);
		TRange<float> VolumeRange = TRange<float>(Data.VolumeRange.Min, Data.VolumeRange.Max);
		OutVolumeMultiplier =  FMath::GetMappedRangeValueClamped(VolumeSpeedRange, VolumeRange, Speed);
	}
	else
	{
		OutVolumeMultiplier = 1.f;
	}

	if (Data.bMapPitch)
	{
		TRange<float> PitchSpeedRange = TRange<float>(Data.PitchSpeedRange.Min, Data.PitchSpeedRange.Max);
		TRange<float> PitchRange = TRange<float>(Data.PitchRange.Min, Data.PitchRange.Max);
		OutPitchMultiplier = FMath::GetMappedRangeValueClamped(PitchSpeedRange, PitchRange, Speed);
	}
	else
	{
		OutPitchMultiplier = 1.f;
	}
}

void ASuraPawnPlayer::OnDamaged()
{
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

	if (DamageCauser)
	{
		USoundBase* HitSound = nullptr;
		if (ASuraCharacterEnemyBase* Enemy = Cast<ASuraCharacterEnemyBase>(DamageCauser))
		{
			FName EnemyType = Enemy->GetEnemyType();
			if (EnemyType == "Melee")
			{
				HitSound = PlayerSound_DataAsset->MeleeEnemyHitSound;
			}
			else if (EnemyType == "Rifle")
			{
				HitSound = PlayerSound_DataAsset->RifleEnemyHitSound;
			}
			else if (EnemyType == "Charger")
			{
				HitSound = PlayerSound_DataAsset->ChargerEnemyHitSound;
			}
			else if (EnemyType == "Turret")
			{
				HitSound = PlayerSound_DataAsset->TurretEnemyHitSound;
			}
		}
		else if (ASuraCharacterBossProto* Boss = Cast<ASuraCharacterBossProto>(DamageCauser))
		{
			switch (DamageData.DamageType)
			{
				case EDamageType::Charge:
					HitSound = PlayerSound_DataAsset->BossDownedHitSound;
					break;
				default:
					HitSound = PlayerSound_DataAsset->BossNormalHitSound;
					break;
			}
		}

		if (HitSound)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, GetActorLocation());
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


void ASuraPawnPlayer::OnDeath()
{
	GEngine->AddOnScreenDebugMessage(10, 15.0f, FColor::Yellow, TEXT("Player Dead"));
	GetPlayerMovementComponent()->NotifyDeath();
	
	// if (ASuraLevelGameMode* GameMode = Cast<ASuraLevelGameMode>(UGameplayStatics::GetGameMode(this)))
	// {
	// 	GameMode->OnPlayerDeath(this);
	// }
}

void ASuraPawnPlayer::OnDash(FVector2D MovementInput)
{
	const FPlayerSoundData& Data = PlayerSound_DataAsset->DashSound;
	float VolumeMultiplier, PitchMultiplier;

	float Speed = GetPlayerMovementComponent()->Velocity.Size();
	
	CalculateMappedSoundValue(Data, Speed, VolumeMultiplier, PitchMultiplier);

	UGameplayStatics::SpawnSoundAttached(Data.Sound, GetRootComponent(), NAME_None,
		FVector(ForceInit), FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset,
		false, VolumeMultiplier, PitchMultiplier);

	if (Data.bDebug)
	{
		if (!GEngine) return;
		GEngine->AddOnScreenDebugMessage(77777, Data.DebugDisplayDuration, FColor::Green,
			FString::Printf(TEXT("Dash Sound Evaluation Speed: %.0f, "
						"Mapped Volume Multiplier: %.3f, Mapped Pitch Multiplier: %.3f"), Speed, VolumeMultiplier, PitchMultiplier));
	}	

	
	if (MovementInput.IsZero())
	{
		ForwardDashEffectComponent->Activate();

		if (BackwardDashEffectComponent->IsActive())
			BackwardDashEffectComponent->Deactivate();
		if (LeftDashEffectComponent->IsActive())
			LeftDashEffectComponent->Deactivate();
		if (RightDashEffectComponent->IsActive())
			RightDashEffectComponent->Deactivate();
	}
	else
	{
		if (MovementInput.Y > 0)
		{
			ForwardDashEffectComponent->Activate();
			
			if (BackwardDashEffectComponent->IsActive())
				BackwardDashEffectComponent->Deactivate();
			if (LeftDashEffectComponent->IsActive())
				LeftDashEffectComponent->Deactivate();
			if (RightDashEffectComponent->IsActive())
				RightDashEffectComponent->Deactivate();
		}
		else if (MovementInput.Y < 0)
		{
			BackwardDashEffectComponent->Activate();

			if (ForwardDashEffectComponent->IsActive())
				ForwardDashEffectComponent->Deactivate();
			if (LeftDashEffectComponent->IsActive())
				LeftDashEffectComponent->Deactivate();
			if (RightDashEffectComponent->IsActive())
				RightDashEffectComponent->Deactivate();
		}
		else
		{
			if (MovementInput.X < 0)
			{
				LeftDashEffectComponent->Activate();
				if (ForwardDashEffectComponent->IsActive())
					ForwardDashEffectComponent->Deactivate();
				if (BackwardDashEffectComponent->IsActive())
					BackwardDashEffectComponent->Deactivate();
				if (RightDashEffectComponent->IsActive())
					RightDashEffectComponent->Deactivate();
			}
			else if (MovementInput.X > 0)
			{
				RightDashEffectComponent->Activate();

				if (ForwardDashEffectComponent->IsActive())
					ForwardDashEffectComponent->Deactivate();
				if (BackwardDashEffectComponent->IsActive())
					BackwardDashEffectComponent->Deactivate();
				if (LeftDashEffectComponent->IsActive())
					LeftDashEffectComponent->Deactivate();
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








