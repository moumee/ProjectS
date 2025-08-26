
#include "ActorComponents/WeaponSystem/ACWeapon.h"
#include "Characters/PawnBasePlayer/SuraPawnPlayer.h"

#include "ActorComponents/WeaponSystem/SuraProjectile.h"
#include "ActorComponents/WeaponSystem/WeaponInterface.h"
#include "ActorComponents/WeaponSystem/WeaponSystemComponent.h"

#include "ActorComponents/WeaponSystem/SuraWeaponBaseState.h"
#include "ActorComponents/WeaponSystem/SuraWeaponIdleState.h"
#include "ActorComponents/WeaponSystem/SuraWeaponFiringState.h"
#include "ActorComponents/WeaponSystem/SuraWeaponUnequippedState.h"
#include "ActorComponents/WeaponSystem/SuraWeaponReloadingState.h"
#include "ActorComponents/WeaponSystem/SuraWeaponPumpActionReloadState.h"
#include "ActorComponents/WeaponSystem/SuraWeaponSwitchingState.h"
#include "ActorComponents/WeaponSystem/SuraWeaponTargetingState.h"
#include "ActorComponents/WeaponSystem/SuraWeaponChargingState.h"
#include "ActorComponents/WeaponSystem/SuraWeaponWaitingState.h"
#include "ActorComponents/WeaponSystem/WeaponCameraShakeBase.h"
#include "ActorComponents/WeaponSystem/AmmoCounterWidget.h"
#include "ActorComponents/WeaponSystem/WeaponAimUIWidget.h"
#include "ActorComponents/WeaponSystem/TargetingSkillWidget.h"


#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Animation/AnimInstance.h"
#include "Engine/LocalPlayer.h" //TODO: ������ ���� header���� �˾ƺ�����
#include "Engine/World.h"

#include "Camera/CameraComponent.h"
#include "Camera/PlayerCameraManager.h"

#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "Components/AudioComponent.h"
#include "Components/SceneCaptureComponent2D.h"

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

//----------------------------------
#include "ActorComponents/UISystem/ACUIMangerComponent.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/CanvasRenderTarget2D.h"
#include "UI/TargetMarkerWidget.h"
#include "UI/UIData.h"

// Sets default values for this component's properties
AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(GetRootComponent(), FName(TEXT("WeaponMesh")));
	WeaponMesh->SetVisibility(false);
	SetMeshVisibility(false);

	WeaponAnimInstance = WeaponMesh->GetAnimInstance();

	//TODO: Muzzle Offset�� weapon Type�� ���� �ٸ���
	// ����� �⺻������ Weapon Mesh��"Muzzle" Socket�� ��ġ�� �̿��ϰ� �־ MuzzleOffset�� ��� ���ϴ� ����
	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);

	// Fire Mode
	//TODO: �ϴ��� �⺻ Single�� �ϴµ�, WeaponName�� ���� �����ڿ��� �������ִ� ������ �����ϱ�
	WeaponName = EWeaponName::WeaponName_Rifle;
	WeaponType = EWeaponType::WeaponType_Rifle;
	
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->SetCollisionObjectType(ECC_GameTraceChannel3); //Weapon
	WeaponMesh->SetCollisionResponseToAllChannels(ECR_Ignore);

	//---------------------------------------------------------------------------------
	LeftAmmoInCurrentMag = MaxAmmoPerMag;

	//<UI>
	//AmmoCounterWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("AmmoCounterWidgetComponent"));
	//AmmoCounterWidgetComponent->SetDrawSize(FVector2D());
	//AmmoCounterWidgetComponent->SetHiddenInGame(false);
}

void AWeapon::InitializeWeapon(ASuraPawnPlayer* NewCharacter)
{
	Character = NewCharacter;
	if (Character)
	{
		CharacterAnimInstance = Character->GetArmMesh()->GetAnimInstance();
		//InitializeCamera(Character);
		LoadWeaponData();
	
		if (Character->GetWeaponSystemComponent()->IsSceneCaptureActive())
		{
			TInlineComponentArray<USceneComponent*> MeshComponents;
			GetComponents<USceneComponent>(MeshComponents);

			for (USceneComponent* MeshComp : MeshComponents)
			{
				if (UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(MeshComp))
				{
					UE_LOG(LogTemp, Error, TEXT("Weapon Mesh"));
					Character->GetSceneCaptureComponent()->ShowOnlyComponent(PrimComp);
					PrimComp->SetVisibleInSceneCaptureOnly(true);
				}
			}
		}
	}
	InitializeUI();

	// Set up action bindings
	CharacterController = Cast<APlayerController>(Character->GetController());
	if (CharacterController)
	{
		//TODO: �Ʒ� �ڵ忡 ���ؼ� �˾ƺ�����. Multi Play�� ���� ���ΰ�?
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(CharacterController->GetLocalPlayer()))
		{
			// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
			Subsystem->AddMappingContext(FireMappingContext, 1);
		}
	}

	SetMeshVisibility(false);
	SetAimSocketRelativeTransform();
}

void AWeapon::InitializeCamera(ASuraPawnPlayer* NewCharacter) //TODO: Need to integrate camera system with player system
{
	if (NewCharacter)
	{
		CamSetting_Default.CameraRelativeLocation = NewCharacter->GetCameraComponent()->GetRelativeLocation();
		CamSetting_Default.CameraRelativeRotation = NewCharacter->GetCameraComponent()->GetRelativeRotation();
		CamSetting_Default.FOV = NewCharacter->GetCameraComponent()->FieldOfView;
	}
}

void AWeapon::InitializeUI()
{
	if (AimUIWidgetClass)
	{
		AimUIWidget = CreateWidget<UWeaponAimUIWidget>(GetWorld(), AimUIWidgetClass);
	}
	if (AmmoCounterWidgetClass)
	{
		AmmoCounterWidget = CreateWidget<UAmmoCounterWidget>(GetWorld(), AmmoCounterWidgetClass);
		if (AmmoCounterWidget)
		{
			AmmoCounterWidget->UpdateAmmoCount(LeftAmmoInCurrentMag);
			AmmoCounterWidget->UpdateTotalAmmo(TotalAmmo);
		}
	}
	if (TargetingSkillWidgetClass)
	{
		TargetingSkillWidget = CreateWidget<UTargetingSkillWidget>(GetWorld(), TargetingSkillWidgetClass);
		if (TargetingSkillWidget)
		{
			TargetingSkillWidget->InitializeUI(TargetingSkillCoolDown, TargetingSkillCoolDown, ElapsedTimeAfterTargetingStarted, MaxTargetingTime);
			TargetingSkillWidget->SetDetectionTimeUIVisible(false);
		}
	}
}

void AWeapon::LoadWeaponData()
{
	WeaponData = WeaponDataTableHandle.GetRow<FWeaponData>("");
	if (WeaponData)
	{
		// <WeaponSocket>
		WeaponSocketName = WeaponData->WeaponSocket;

		// <Skill>
		bIsSkillWeapon = WeaponData->bIsSkillWeapon;
		bAllowNormalFireForSkillWeapon = WeaponData->bAllowNormalFireForSkillWeapon;

		// <Action>
		LeftMouseAction = WeaponData->LeftMouseAction;
		RightMouseAction = WeaponData->RightMouseAction;
		SkillAction = WeaponData->SkillAction;

		// <Projectile Class>
		FireData_L.ProjectileClass = WeaponData->LeftProjectileClass;
		FireData_R.ProjectileClass = WeaponData->RightProjectileClass;
		FireData_Skill.ProjectileClass = WeaponData->SkillProjectileClass;

		// <Sound>
		ChargeSound = WeaponData->ChargeSound;

		FireData_L.FireSound = WeaponData->FireSound_L;
		FireData_R.FireSound = WeaponData->FireSound_R;
		FireData_Skill.FireSound = WeaponData->FireSound_Skill;

		TargetSearchLoopSound = WeaponData->TargetSearchLoopSound;
		TargetLockedSound = WeaponData->TargetLockedSound;

		// <Effect>
		ChargeEffect = WeaponData->ChargeEffect;
		ChargeEffectLocation = WeaponData->ChargeEffectLocation;
		ChargeEffectRotation = WeaponData->ChargeEffectRotation;
		ChargeEffenctScale = WeaponData->ChargeEffenctScale;

		FireData_L.MuzzleFireEffect = WeaponData->FireEffect_L;
		FireData_R.MuzzleFireEffect = WeaponData->FireEffect_R;
		FireData_Skill.MuzzleFireEffect = WeaponData->FireEffect_Skill;

		// <Reload>
		ReloadingTime = WeaponData->ReloadingTime;
		PumpReloadingTime_Start = WeaponData->PumpReloadingTime_Start;
		PumpReloadingTime_Loop = WeaponData->PumpReloadingTime_Loop;
		PumpReloadingTime_End = WeaponData->PumpReloadingTime_End;
		PumpReloadingTime_LoopToFire = WeaponData->PumpReloadingTime_LoopToFire;
		MaxTotalAmmo = WeaponData->MaxTotalAmmo;
		TotalAmmo = MaxTotalAmmo;
		MaxAmmoPerMag = WeaponData->MaxAmmoPerMag;
		LeftAmmoInCurrentMag = MaxAmmoPerMag;
		FireData_L.AmmoCost = WeaponData->AmmoConsumedPerShot_Left;
		FireData_R.AmmoCost = WeaponData->AmmoConsumedPerShot_Right;
		FireData_Skill.AmmoCost = WeaponData->AmmoCost_Skill;
		FireData_L.bAllowFireWithInsufficientAmmo = WeaponData->bAllowFireWithInsufficientAmmo_L;
		FireData_R.bAllowFireWithInsufficientAmmo = WeaponData->bAllowFireWithInsufficientAmmo_R;
		FireData_Skill.bAllowFireWithInsufficientAmmo = WeaponData->bAllowFireWithInsufficientAmmo_Skill;
		bActivePumpActionReload = WeaponData->bActivePumpActionReload;

		// <HitScan>
		bIsHitScan_L = WeaponData->bIsHitScan_Left;
		bIsHitScan_R = WeaponData->bIsHitScan_Right;

		// <SingleShot>
		SingleShotDelay = WeaponData->SingleShotDelay;

		// <BurstShot>
		BurstShotDelay = WeaponData->BurstShotDelay;
		BurstShotFireRate = WeaponData->BurstShotFireRate;
		BurstShotCount = WeaponData->BurstShotCount;

		// <FullAutoShot>
		FullAutoShotFireRate = WeaponData->FullAutoShotFireRate;

		// <ProjectileSpread>
		DefaultSpread = WeaponData->DefaultSpread;
		ZoomSpread = WeaponData->ZoomSpread;

		// <MultiProjectileSpread>
		bEnableMultiProjectile_L = WeaponData->bEnableMultiProjectile_Left;
		bEnableMultiProjectile_R = WeaponData->bEnableMultiProjectile_Right;
		FireData_L.MaxAngleOfMultiProjectileSpread = WeaponData->MaxAngleOfMultiProjectileSpread_L;
		FireData_R.MaxAngleOfMultiProjectileSpread = WeaponData->MaxAngleOfMultiProjectileSpread_R;
		FireData_L.PelletsNum = WeaponData->PelletsNum_L;
		FireData_R.PelletsNum = WeaponData->PelletsNum_R;

		// <Recoil>
		ZoomRecoil = WeaponData->ZoomRecoil;

		FireData_L.Recoil = WeaponData->DefaultRecoil_L;
		FireData_R.Recoil = WeaponData->DefaultRecoil_R;
		FireData_Skill.Recoil = WeaponData->DefaultRecoil_Skill;

		// <ArmRecoil Animation>
		FireData_L.Armrecoil = WeaponData->ArmRecoil_L;
		FireData_R.Armrecoil = WeaponData->ArmRecoil_R;
		FireData_Skill.Armrecoil = WeaponData->ArmRecoil_Skill;
		ArmRecoil_Hand = WeaponData->ArmRecoil_Hand;
		ArmRecoil_UpperArm = WeaponData->ArmRecoil_UpperArm;
		ArmRecoil_LowerArm = WeaponData->ArmRecoil_LowerArm;

		// <Animation>
		RightHandSocketTransform = WeaponData->RightHandSocketTransform;
		RightHandSocketTransform_Crouch = WeaponData->RightHandSocketTransform_Crouch;
		RightHandSocketTransform_Targeting = WeaponData->RightHandSocketTransform_Targeting;
		RightHandSocketTransform_Targeting_Crouch = WeaponData->RightHandSocketTransform_Targeting_Crouch;
		SkillWeaponSocketTransform_Active = WeaponData->SkillWeaponSocketTransform_Active;
		SkillWeaponSocketTransform_Inactive = WeaponData->SkillWeaponSocketTransform_Inactive;

		// <Camera Shake>
		ZoomCameraShakeClass = WeaponData->ZoomCameraShakeClass;
		ChargingCameraShakeClass = WeaponData->ChargingCameraShakeClass;

		FireData_L.CamShake = WeaponData->DefaultCameraShakeClass_L;
		FireData_R.CamShake = WeaponData->DefaultCameraShakeClass_R;
		FireData_Skill.CamShake = WeaponData->DefaultCameraShakeClass_Skill;

		// <Targeting(Homing)>
		MissileLaunchDelay = WeaponData->MissileLaunchDelay;
		MaxTargetNum = WeaponData->MaxTargetNum;
		MaxTargetDetectionRadius = WeaponData->MaxTargetDetectionRadius;
		MaxTargetDetectionAngle = WeaponData->MaxTargetDetectionAngle;
		MaxTargetDetectionTime = WeaponData->MaxTargetDetectionTime;
		TimeToReachMaxTargetDetectionRange = WeaponData->TimeToReachMaxTargetDetectionRange;
		TargetingGlobalTimeScale = WeaponData->TargetingGlobalTimeScale;
		TargetingGlobalTimeDilationSpeed_In = WeaponData->TargetingGlobalTimeDilationSpeed_In;
		TargetingGlobalTimeDilationSpeed_Out = WeaponData->TargetingGlobalTimeDilationSpeed_Out;
		TargetingSkillCoolDown = WeaponData->TargetingSkillCoolDown;
		MaxTargetingTime = WeaponData->MaxTargetingTime;

		// <Charging>
		bAutoFireAtMaxChargeTime = WeaponData->bAutoFireAtMaxChargeTime;
		ChargeTimeThreshold = WeaponData->ChargeTimeThreshold;
		MaxChargeTime = WeaponData->MaxChargeTime;
		ChargingAdditionalDamageBase = WeaponData->ChargingAdditionalDamageBase;
		ChargingAdditionalRecoilAmountPitchBase = WeaponData->ChargingAdditionalRecoilAmountPitchBase;
		ChargingAdditionalRecoilAmountYawBase = WeaponData->ChargingAdditionalRecoilAmountYawBase;
		ChargingAdditionalProjectileRadiusBase = WeaponData->ChargingAdditionalProjectileRadiusBase;
		ChargingAdditionalPelletMaxNum = WeaponData->ChargingAdditionalPelletMaxNum;

		// <Penetration>
		MaxPenetrableObjectsNum_Left = WeaponData->MaxPenetrableObjectsNum_Left;
		MaxPenetrableObjectsNum_Right = WeaponData->MaxPenetrableObjectsNum_Right;

		// <IK>
		RightHandOffset = WeaponData->RightHandOffset;
	}
}

void AWeapon::SetMeshVisibility(bool bflag)
{
	TInlineComponentArray<USceneComponent*> MeshComponents;
	GetComponents<USceneComponent>(MeshComponents);
	for (USceneComponent* MeshComp : MeshComponents)
	{
		if (MeshComp)
		{
			MeshComp->SetVisibility(bflag, true);
		}
	}
}

// Called when the game starts
void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	//TODO: WeaponType�� ���� �����Ǵ� State ��ü���� �޶������� �����ϱ�
	IdleState = NewObject<USuraWeaponIdleState>(this, USuraWeaponIdleState::StaticClass());
	FiringState = NewObject<USuraWeaponFiringState>(this, USuraWeaponFiringState::StaticClass());
	UnequippedState = NewObject<USuraWeaponUnequippedState>(this, USuraWeaponUnequippedState::StaticClass());
	ReloadingState = NewObject<USuraWeaponReloadingState>(this, USuraWeaponReloadingState::StaticClass());
	PumpActionReloadingState = NewObject<USuraWeaponPumpActionReloadState>(this, USuraWeaponPumpActionReloadState::StaticClass());
	SwitchingState = NewObject<USuraWeaponSwitchingState>(this, USuraWeaponSwitchingState::StaticClass());

	TargetingState = NewObject<USuraWeaponTargetingState>(this, USuraWeaponTargetingState::StaticClass());
	ChargingState = NewObject<USuraWeaponChargingState>(this, USuraWeaponChargingState::StaticClass());
	WaitingState = NewObject<USuraWeaponWaitingState>(this, USuraWeaponWaitingState::StaticClass());

	WeaponAnimInstance = WeaponMesh->GetAnimInstance();

	CurrentState = UnequippedState;

	//TODO: BeginPlay���� ChangeState�� ����ϸ�, ChangeState -> EnterState -> ���⿡ ���� ���� �� ������ �߻���
	// �������� ������ �� ������, ���� ��Ȯ�� ������ ã�� ������
	//ChangeState(UnequippedState);


	//SetCollisionObjectType(ECC_EngineTraceChannel2);

	//SetCollisionProfileName(FName("Weapon"));
	//SetCollisionResponseToAllChannels(ECR_Ignore);
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (CurrentState)
	{
		CurrentState->UpdateState(this, DeltaTime);
	}
	UpdateRecoil(DeltaTime);
	UpdateSpread(DeltaTime);
	UpdateGlobalTimeDiation(DeltaTime); //TODO: If the player tries to switch weapons, reset the global time dilation to its default value
	UpdateTargetingSkillUI(); //TODO: Should Update UI When even TargetingSkillTimer is not activated

	UpdateOverheat(DeltaTime); //TODO: Overheat ����� �Ⱦ��Ŷ�� �ϴµ�, Ȥ�� mesh�� ���� ȿ�� ������ ���� �����ϱ� �ϴ��� ����

	//Suhyeon
	if (CurrentState == TargetingState)
	{
		// 모든 위젯의 수동 타이머를 업데이트
		for (UUserWidget* Widget : TargetMarkerWidgets)
		{
			UTargetMarkerWidget* TargetMarker = Cast<UTargetMarkerWidget>(Widget);
			if (TargetMarker)
			{
				float RealDelta = FApp::GetDeltaTime(); // 슬로우타임 무시
				
				// 위젯 자체의 타이머 업데이트 함수를 호출함
				TargetMarker->UpdateLockOnTimer(RealDelta);
			}
		}
	}
}

void AWeapon::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

bool AWeapon::AttachWeaponToPlayer(ASuraPawnPlayer* TargetCharacter)
{
	Character = TargetCharacter;
	if (Character == nullptr) { return false; }

	// Attach the weapon to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	

	if (WeaponSocketName.IsNone())
	{
		AttachToComponent(Character->GetArmMesh(), AttachmentRules, FName(TEXT("Gun")));
	}
	else
	{
		AttachToComponent(Character->GetArmMesh(), AttachmentRules, WeaponSocketName);
	}

	// Seunghwan - use unreal first person rendering
	WeaponMesh->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;
	TArray<USceneComponent*> AttachedChildren;
	WeaponMesh->GetChildrenComponents(true, AttachedChildren);
	for (auto Child : AttachedChildren)
	{
		if (UPrimitiveComponent* PrimitiveChild = Cast<UPrimitiveComponent>(Child))
		{
			PrimitiveChild->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;
		}
	}
	

	//---------------------------------------------
	//TODO: �� ���� ����� ������ ������
	//RightHandToAimSocketOffset = this->GetSocketLocation(FName(TEXT("Aim"))) - Character->GetMesh()->GetSocketLocation(FName("Gun"));
	RightHandToAimSocketOffset = WeaponMesh->GetSocketLocation(FName(TEXT("Aim"))) - Character->GetArmMesh()->GetBoneLocation(FName(TEXT("hand_r")));
	if (Character->GetWeaponSystemComponent() && Character->GetWeaponSystemComponent()->GetClass()->ImplementsInterface(UWeaponInterface::StaticClass()))
	{
		Character->GetWeaponSystemComponent()->SetRightHandToAimSocketOffset(RightHandToAimSocketOffset);
	}

	// Set AimSocketTransform
	SetAimSocketTransform();

	// Set Up Widget UI Class
	if (bIsSkillWeapon) { ActivateAimUIWidget(false); }
	else { ActivateAimUIWidget(true); }
	ActivateAmmoCounterWidget(true);
	ActivateTargetingSkillWidget(true);

	SetMeshVisibility(true);
	return true;
}

void AWeapon::DetachWeaponFromPlayer()
{
	if (Character == nullptr)
	{
		return;
	}
	else
	{
		// Seunghwan - disable first person rendering
		WeaponMesh->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::None;
		TArray<USceneComponent*> AttachedChildren;
		WeaponMesh->GetChildrenComponents(true, AttachedChildren);
		for (auto Child : AttachedChildren)
		{
			if (UPrimitiveComponent* PrimitiveChild = Cast<UPrimitiveComponent>(Child))
			{
				PrimitiveChild->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::None;
			}
		}
		
		//ActivateCrosshairWidget(false);
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		//WeaponMesh->SetVisibility(false);
		SetMeshVisibility(false);
	}
}

void AWeapon::FireSingleProjectile(FWeaponFireData* FireData, int32 NumPenetrable, float AdditionalDamage, float AdditionalRecoilAmountPitch, float AdditionalRecoilAmountYaw, float AdditionalProjectileRadius, bool bIsHoming, AActor* HomingTarget)
{
	if (CurrentState != UnequippedState)
	{
		if (Character == nullptr || Character->GetController() == nullptr || FireData == nullptr)
		{
			return;
		}

		if (FireData->AmmoCost > 0)
		{
			if (FireData->bAllowFireWithInsufficientAmmo)
			{
				if (LeftAmmoInCurrentMag <= 0)
				{
					return;
				}
			}
			else
			{
				if (!HasAmmoInCurrentMag(FireData->AmmoCost))
				{
					return;
				}
			}
			ConsumeAmmo(FireData->AmmoCost, FireData->bAllowFireWithInsufficientAmmo);
		}

		FVector LineTraceStartLocation = Character->GetCameraComponent()->GetComponentLocation();
		FVector LineTraceDirection = Character->GetCameraComponent()->GetForwardVector();

		if (bIsZoomIn)
		{
			if (ZoomSpread.bEnableProjectileSpread)
			{
				LineTraceDirection = GetRandomSpreadVector(Character->GetCameraComponent()->GetForwardVector());
			}

			if (ZoomSpread.bEnableProjectileSpread || ZoomSpread.bEnableAimUISpread)
			{
				AddSpreadValue(&ZoomSpread);
			}
		}
		else
		{
			if (DefaultSpread.bEnableProjectileSpread)
			{
				LineTraceDirection = GetRandomSpreadVector(Character->GetCameraComponent()->GetForwardVector());
			}
			if (DefaultSpread.bEnableProjectileSpread || DefaultSpread.bEnableAimUISpread)
			{
				AddSpreadValue(&DefaultSpread);
			}
		}

		FVector LineTraceHitLocation;

		//----------------------------------------------
		//FVector LineTraceStartLocation;
		//FVector LineTraceDirection;
		//FVector LineTraceHitLocation;
		//CalculateScreenCenterWorldPositionAndDirection(LineTraceStartLocation, LineTraceDirection);

		if (PerformLineTrace(LineTraceStartLocation, LineTraceDirection, LineTraceMaxDistance, LineTraceHitLocation))
		{
			TargetLocationOfProjectile = LineTraceHitLocation;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("LineTrace Failed!!!!!!!!!!!!!"));
			TargetLocationOfProjectile = LineTraceHitLocation;
		}

		// Try and fire a projectile
		if (FireData != nullptr && FireData->ProjectileClass != nullptr)
		{
			UWorld* const World = GetWorld();
			if (World != nullptr)
			{
				const FVector SpawnLocation = WeaponMesh->GetSocketLocation(FName(TEXT("Muzzle")));
				const FRotator SpawnRotation = (TargetLocationOfProjectile - SpawnLocation).Rotation();

				//Set Spawn Collision Handling Override
				FActorSpawnParameters ActorSpawnParams;
				ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				// Spawn the projectile at the muzzle
				ASuraProjectile* Projectile = World->SpawnActor<ASuraProjectile>(FireData->ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
				Projectile->InitializeProjectile(Character, this, AdditionalDamage, AdditionalProjectileRadius, NumPenetrable);
				SetUpAimUIDelegateBinding(Projectile);
				if (bIsHoming)
				{
					Projectile->SetHomingTarget(bIsHoming, HomingTarget);
				}
				Projectile->LaunchProjectile();

				SpawnMuzzleFireEffect(FireData->MuzzleFireEffect, SpawnLocation, SpawnRotation);
			}
		}

		// Try and play the sound if specified
		if (FireData != nullptr && FireData->FireSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(this, FireData->FireSound, Character->GetActorLocation());
		}

		StartFireAnimation(AM_Fire_Character, AM_Fire_Weapon);

		// <Overheat>
		if (bIsOverheatMode)
		{
			AddOverheatValue();
		}

		// <Recoil & CamShake>
		if (bIsZoomIn)
		{
			AddRecoilValue(&ZoomRecoil, AdditionalRecoilAmountPitch, AdditionalRecoilAmountYaw);
			ApplyCameraShake(ZoomCameraShakeClass);
		}
		else
		{
			AddRecoilValue(&FireData->Recoil, AdditionalRecoilAmountPitch, AdditionalRecoilAmountYaw);
			ApplyCameraShake(FireData->CamShake);
		}

		// <ArmRecoil Animation>
		if (bIsSkillWeapon) { AddSkillWeaponRecoil(&FireData->Armrecoil); }
		else { AddArmRecoil(&FireData->Armrecoil); }
	}
}

void AWeapon::FireMultiProjectile(FWeaponFireData* FireData, int32 NumPenetrable, float AdditionalDamage, float AdditionalRecoilAmountPitch, float AdditionalRecoilAmountYaw, float AdditionalProjectileRadius, int32 AdditionalPellet, bool bIsHoming, AActor* HomingTarget)
{
	if (CurrentState != UnequippedState)
	{
		if (Character == nullptr || Character->GetController() == nullptr || FireData == nullptr)
		{
			return;
		}

		if (FireData->AmmoCost > 0)
		{
			if (FireData->bAllowFireWithInsufficientAmmo)
			{
				if (LeftAmmoInCurrentMag <= 0)
				{
					return;
				}
			}
			else
			{
				if (!HasAmmoInCurrentMag(FireData->AmmoCost))
				{
					return;
				}
			}
			ConsumeAmmo(FireData->AmmoCost, FireData->bAllowFireWithInsufficientAmmo);
		}

		if (bIsZoomIn)
		{
			if (ZoomSpread.bEnableAimUISpread)
			{
				AddSpreadValue(&ZoomSpread);
			}
		}
		else
		{
			if (DefaultSpread.bEnableAimUISpread)
			{
				AddSpreadValue(&DefaultSpread);
			}
		}

		FVector LineTraceStartLocation = Character->GetCameraComponent()->GetComponentLocation();
		FVector LineTraceDirection = Character->GetCameraComponent()->GetForwardVector();
		FVector LineTraceHitLocation;

		if (PerformSphereTrace(LineTraceStartLocation, LineTraceDirection, LineTraceMaxDistance, SphereTraceRadius, LineTraceHitLocation))
		{
			TargetLocationOfProjectile = LineTraceHitLocation;
		}
		else
		{
			TargetLocationOfProjectile = LineTraceHitLocation;
		}

		// <Fire Projectile>
		if (FireData != nullptr && FireData->ProjectileClass != nullptr)
		{
			UWorld* const World = GetWorld();
			if (World != nullptr)
			{
				const FVector SpawnLocation = WeaponMesh->GetSocketLocation(FName(TEXT("Muzzle")));

				for (int pellet = 0; pellet < (FireData->PelletsNum + AdditionalPellet); pellet++)
				{
					const FRotator SpawnRotation = UKismetMathLibrary::RandomUnitVectorInConeInDegrees((TargetLocationOfProjectile - SpawnLocation).GetSafeNormal(), FireData->MaxAngleOfMultiProjectileSpread).Rotation();

					FActorSpawnParameters ActorSpawnParams;
					ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

					ASuraProjectile* Projectile = World->SpawnActor<ASuraProjectile>(FireData->ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
					Projectile->InitializeProjectile(Character, this, AdditionalDamage, AdditionalProjectileRadius, NumPenetrable);
					SetUpAimUIDelegateBinding(Projectile);
					Projectile->LaunchProjectile();

					//TODO: ���߿��� Multi Homing Projectile�� �����ϰ� ���鿹��. �ڵ� Ÿ��������
				}
				//UE_LOG(LogTemp, Error, TEXT("Pellet Num: %d"), (FireData->PelletsNum + AdditionalPellet));
				SpawnMuzzleFireEffect(FireData->MuzzleFireEffect, SpawnLocation, (TargetLocationOfProjectile - SpawnLocation).GetSafeNormal().Rotation());
			}
		}

		// <Sound>
		if (FireData != nullptr && FireData->FireSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(this, FireData->FireSound, Character->GetActorLocation());
		}

		StartFireAnimation(AM_Fire_Character, AM_Fire_Weapon); //TODO: ������. ����

		// <Overheat>
		if (bIsOverheatMode)
		{
			AddOverheatValue();
		}

		// <Recoil & CamShake>
		if (bIsZoomIn)
		{
			AddRecoilValue(&ZoomRecoil, AdditionalRecoilAmountPitch, AdditionalRecoilAmountYaw);
			ApplyCameraShake(ZoomCameraShakeClass);
		}
		else
		{
			AddRecoilValue(&FireData->Recoil, AdditionalRecoilAmountPitch, AdditionalRecoilAmountYaw);
			ApplyCameraShake(FireData->CamShake);
		}

		// <ArmRecoil Animation>
		AddArmRecoil(&FireData->Armrecoil);
	}
}

void AWeapon::FireSingleHitScan(FWeaponFireData* FireData, int32 NumPenetrable, float AdditionalDamage, float AdditionalRecoilAmountPitch, float AdditionalRecoilAmountYaw, float AdditionalProjectileRadius)
{
	UE_LOG(LogTemp, Error, TEXT("FireSingleHitScan!"));

	if (CurrentState != UnequippedState)
	{
		if (Character == nullptr || Character->GetController() == nullptr)
		{
			return;
		}

		if (FireData->AmmoCost > 0)
		{
			if (FireData->bAllowFireWithInsufficientAmmo)
			{
				if (LeftAmmoInCurrentMag <= 0)
				{
					return;
				}
			}
			else
			{
				if (!HasAmmoInCurrentMag(FireData->AmmoCost))
				{
					return;
				}
			}
			ConsumeAmmo(FireData->AmmoCost, FireData->bAllowFireWithInsufficientAmmo);
		}

		FVector LineTraceStartLocation = Character->GetCameraComponent()->GetComponentLocation();
		FVector LineTraceDirection = Character->GetCameraComponent()->GetForwardVector();

		if (bIsZoomIn)
		{
			if (ZoomSpread.bEnableProjectileSpread)
			{
				LineTraceDirection = GetRandomSpreadVector(Character->GetCameraComponent()->GetForwardVector());
			}

			if (ZoomSpread.bEnableProjectileSpread || ZoomSpread.bEnableAimUISpread)
			{
				AddSpreadValue(&ZoomSpread);
			}
		}
		else
		{
			if (DefaultSpread.bEnableProjectileSpread)
			{
				LineTraceDirection = GetRandomSpreadVector(Character->GetCameraComponent()->GetForwardVector());
			}

			if (DefaultSpread.bEnableProjectileSpread || DefaultSpread.bEnableAimUISpread)
			{
				AddSpreadValue(&DefaultSpread);
			}
		}

		FVector LineTraceHitLocation;

		//----------------------------------------------

		if (PerformLineTrace(LineTraceStartLocation, LineTraceDirection, LineTraceMaxDistance, LineTraceHitLocation))
		{
			TargetLocationOfProjectile = LineTraceHitLocation;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("LineTrace Failed!!!!!!!!!!!!!"));
			TargetLocationOfProjectile = LineTraceHitLocation;
		}

		// Try and fire a projectile
		if (FireData != nullptr && FireData->ProjectileClass != nullptr)
		{
			UWorld* const World = GetWorld();
			if (World != nullptr)
			{
				const FVector SpawnLocation = WeaponMesh->GetSocketLocation(FName(TEXT("Muzzle")));
				const FRotator SpawnRotation = (TargetLocationOfProjectile - SpawnLocation).Rotation();

				//Set Spawn Collision Handling Override
				FActorSpawnParameters ActorSpawnParams;
				ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				// Spawn the projectile at the muzzle
				ASuraProjectile* Projectile = World->SpawnActor<ASuraProjectile>(FireData->ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
				Projectile->InitializeProjectile(Character, this, AdditionalDamage, AdditionalProjectileRadius, NumPenetrable, true);
				SetUpAimUIDelegateBinding(Projectile);

				Projectile->SetHomingTarget(false, nullptr);

				//Projectile->LaunchProjectile(); //TODO: not to use Collision Check
				Projectile->LaunchHitScan(LineTraceStartLocation, LineTraceDirection);

				SpawnMuzzleFireEffect(FireData->MuzzleFireEffect, SpawnLocation, SpawnRotation);
			}
		}

		// Try and play the sound if specified
		if (FireData != nullptr && FireData->FireSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(this, FireData->FireSound, Character->GetActorLocation());
		}

		StartFireAnimation(AM_Fire_Character, AM_Fire_Weapon);

		// <Recoil & CamShake>
		if (bIsZoomIn)
		{
			AddRecoilValue(&ZoomRecoil, AdditionalRecoilAmountPitch, AdditionalRecoilAmountYaw);
			ApplyCameraShake(ZoomCameraShakeClass);
		}
		else
		{
			AddRecoilValue(&FireData->Recoil, AdditionalRecoilAmountPitch, AdditionalRecoilAmountYaw);
			ApplyCameraShake(FireData->CamShake);
		}

		// <ArmRecoil Animation>
		AddArmRecoil(&FireData->Armrecoil);
	}
}

void AWeapon::ZoomToggle()
{
	if (CurrentState == IdleState || CurrentState == FiringState)
	{
		UE_LOG(LogTemp, Warning, TEXT("Zoom!!!"));

		if (bIsZoomIn)
		{
			ZoomOut();
		}
		else
		{
			ZoomIn();
		}
	}
}

void AWeapon::ZoomIn()
{
	bIsZoomIn = true;

	//TODO: �Ʒ����� ��Ÿ�� ���� �߻�����. �����ؾ���
	if (Character->GetWeaponSystemComponent() && Character->GetWeaponSystemComponent()->GetClass()->ImplementsInterface(UWeaponInterface::StaticClass()))
	{
		Character->GetWeaponSystemComponent()->ZoomIn(true);
		StartCameraSettingChange(&CamSetting_ZoomIn);
	}
	ActivateAimUIWidget(false);
}

void AWeapon::ZoomOut()
{
	bIsZoomIn = false;

	if (Character->GetWeaponSystemComponent() && Character->GetWeaponSystemComponent()->GetClass()->ImplementsInterface(UWeaponInterface::StaticClass()))
	{
		Character->GetWeaponSystemComponent()->ZoomIn(false);
		StartCameraSettingChange(&CamSetting_Default);
	}
	ActivateAimUIWidget(true);
}

#pragma region WeaponState
void AWeapon::ChangeState(USuraWeaponBaseState* NewState)
{
	if (!NewState || NewState == CurrentState) return;

	if (CurrentState)
	{
		CurrentState->ExitState(this);
	}
	CurrentState = NewState;
	CurrentState->EnterState(this);
}
#pragma endregion

#pragma region Animation
void AWeapon::StartFireAnimation(UAnimMontage* CharacterFireAnimation, UAnimMontage* WeaponFireAnimation)
{
	if (CharacterAnimInstance != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Fire Character Animation!!!"));

		if (!CharacterAnimInstance->Montage_IsPlaying(CharacterFireAnimation))
		{
			CharacterAnimInstance->Montage_Play(CharacterFireAnimation, 0.5f);
		}
	}

	if (WeaponAnimInstance != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Fire Weapon Animation!!!"));
		//WeaponAnimInstance->Montage_Play(WeaponFireAnimation, 1.f);
		if (!WeaponMesh->GetAnimInstance()->Montage_IsPlaying(WeaponFireAnimation))
		{
			WeaponMesh->GetAnimInstance()->Montage_Play(WeaponFireAnimation, 1.f);
		}
	}
}
void AWeapon::StartAnimation(UAnimMontage* CharacterAnimation, UAnimMontage* WeaponAnimation, float CharacterAnimPlayRate, float WeaponAnimPlayRate, FName StartSection)
{
	if (CharacterAnimInstance != nullptr && CharacterAnimation != nullptr)
	{
		if (!CharacterAnimInstance->Montage_IsPlaying(CharacterAnimation))
		{
			//TODO: Blend ���� ����� �ؾ���
			//FMontageBlendSettings BlendSettings;
			//BlendSettings.Blend.BlendTime = 0.1f;

			CharacterAnimation->BlendIn.SetBlendOption(EAlphaBlendOption::Linear);
			CharacterAnimation->BlendIn.SetAlpha(10.f);
			CharacterAnimation->BlendOut.SetBlendOption(EAlphaBlendOption::Linear);
			CharacterAnimation->BlendOut.SetAlpha(10.f);
	
			CharacterAnimInstance->Montage_Play(CharacterAnimation, CharacterAnimation->GetPlayLength() / CharacterAnimPlayRate);
			if (!StartSection.IsNone())
			{
				CharacterAnimInstance->Montage_JumpToSection(StartSection, CharacterAnimation);
			}
		}
	}

	if (WeaponAnimInstance != nullptr && WeaponAnimation != nullptr)
	{
		if (!WeaponMesh->GetAnimInstance()->Montage_IsPlaying(WeaponAnimation))
		{
			WeaponMesh->GetAnimInstance()->Montage_Play(WeaponAnimation, WeaponAnimation->GetPlayLength() / WeaponAnimPlayRate);
			if (!StartSection.IsNone())
			{
				WeaponMesh->GetAnimInstance()->Montage_JumpToSection(StartSection, WeaponAnimation);
			}
		}
	}
}
void AWeapon::CancelAnimation(UAnimMontage* CharacterAnimation, UAnimMontage* WeaponAnimation)
{
	if (CharacterAnimInstance != nullptr)
	{
		if (CharacterAnimInstance->Montage_IsPlaying(CharacterAnimation))
		{
			CharacterAnimInstance->Montage_Stop(0.f, CharacterAnimation);
		}
	}

	if (WeaponAnimInstance != nullptr)
	{
		if (WeaponMesh->GetAnimInstance()->Montage_IsPlaying(WeaponAnimation))
		{
			WeaponMesh->GetAnimInstance()->Montage_Stop(0.f, WeaponAnimation);
		}
	}
}
#pragma endregion


void AWeapon::PlayWeaponSound(USoundBase* weaponsound)
{
	if (weaponsound)
	{
		WeaponAudioComponent = UGameplayStatics::SpawnSoundAttached(weaponsound, GetRootComponent(), FName(TEXT("Muzzle")), FVector(0, 0, 0), EAttachLocation::KeepRelativeOffset);
	}
}
void AWeapon::StopWeaponSound()
{
	if (WeaponAudioComponent)
	{
		WeaponAudioComponent->Stop();
	}
}

#pragma region Niagara
void AWeapon::SpawnMuzzleFireEffect(UNiagaraSystem* FireEffect, FVector SpawnLocation, FRotator SpawnRotation)
{
	if (FireEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(), 
			FireEffect,
			SpawnLocation, 
			SpawnRotation, 
			FVector(1.0f),
			true);
	}
}
void AWeapon::SpawnChargeEffect(FVector SpawnLocation, FRotator SpawnRotation, FVector EffectScale)
{
	if (ChargeEffect)
	{
		ChargeEffectComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
			ChargeEffect,
			WeaponMesh,
			FName(TEXT("Muzzle")),
			SpawnLocation,
			FRotator(0, 0, 0),
			EAttachLocation::KeepRelativeOffset,
			true);

		ChargeEffectComponent->SetRelativeScale3D(EffectScale);

		//TODO: We need to find a way to fix the speed. This method doesn't work.
		ChargeEffectComponent->SetSeekDelta(20.f / MaxChargeTime);
	}
}
void AWeapon::DestroyChargeEffect()
{
	if (ChargeEffectComponent)
	{
		ChargeEffectComponent->Deactivate();
		ChargeEffectComponent->DestroyComponent();
		ChargeEffectComponent = nullptr;
	}
}
#pragma endregion


bool AWeapon::PerformLineTrace(FVector StartLocation, FVector LineDirection, float MaxDistance, FVector& HitLocation)
{
	FVector Start = StartLocation;
	FVector End = StartLocation + LineDirection * MaxDistance;

	FHitResult HitResult;

	FCollisionQueryParams Params;
	Params.AddIgnoredComponent(WeaponMesh);
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(Character);

	FCollisionResponseParams ResponseParams;
	ResponseParams.CollisionResponse.SetResponse(ECC_GameTraceChannel1, ECR_Ignore);
	ResponseParams.CollisionResponse.SetResponse(ECC_GameTraceChannel3, ECR_Ignore);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,           // �浹 ��� ����
		Start,               // ���� ����
		End,                 // �� ����
		ECC_Visibility,      // �浹 ä��
		Params,              // ���� �Ű�����
		ResponseParams
	);

	if (bHit)
	{
		HitLocation = HitResult.ImpactPoint;
	}
	else
	{
		HitLocation = End;
	}

	return bHit;
}

bool AWeapon::PerformSphereTrace(FVector StartLocation, FVector TraceDirection, float MaxDistance, float SphereRadius, FVector& HitLocation)
{
	FVector Start = StartLocation;
	FVector End = StartLocation + TraceDirection * MaxDistance;

	FHitResult HitResult;

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Visibility);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_PhysicsBody);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

	FCollisionQueryParams Params;
	Params.AddIgnoredComponent(WeaponMesh);
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(Character);

	bool bHit = GetWorld()->SweepSingleByObjectType(
		HitResult,
		Start,
		End,
		FQuat::Identity,
		ObjectQueryParams,
		FCollisionShape::MakeSphere(SphereRadius),
		Params
	);

	if (bHit)
	{
		FVector TraceEndedPoint = Start + TraceDirection * FVector::Distance(Start, End) * HitResult.Time;

		//DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 1.f, 12, FColor::Red, false, 50.f);
		//DrawDebugSphere(GetWorld(), TraceEndedPoint, 1.f, 12, FColor::Blue, false, 50.f);

		//UE_LOG(LogTemp, Warning, TEXT("phere Trace Hit: %s"), *HitResult.GetActor()->GetName());
		//HitLocation = HitResult.ImpactPoint;
		HitLocation = TraceEndedPoint;
	}
	else
	{
		HitLocation = End;
	}

	return bHit;
}

FVector AWeapon::CalculateScreenCenterWorldPositionAndDirection(FVector& OutWorldPosition, FVector& OutWorldDirection) const
{
	if (!CharacterController || !CharacterController->PlayerCameraManager)
	{
		return FVector::ZeroVector;
	}

	// ȭ�� ũ�� ��������
	FVector2D ViewportSize = GEngine->GameViewport->Viewport->GetSizeXY();

	// ȭ�� �߾� ��ǥ ���
	FVector2D ScreenCenter(ViewportSize.X / 2.0f, ViewportSize.Y / 2.0f);

	// ȭ�� �߽��� ���� ��ġ�� ���� ��������
	FVector WorldPosition, WorldDirection;
	if (CharacterController->DeprojectScreenPositionToWorld(ScreenCenter.X, ScreenCenter.Y, WorldPosition, WorldDirection))
	{
		OutWorldPosition = WorldPosition;
		OutWorldDirection = WorldDirection;
		return WorldPosition + (WorldDirection * 15.0f); // TODO: ���� �Ÿ��� �������. ��ġ ���� �����ϵ��� �����ϱ�
	}

	return FVector::ZeroVector;
}

void AWeapon::SetAimSocketTransform()
{
	FTransform AimSocketTransform = WeaponMesh->GetSocketTransform(FName(TEXT("Aim")));
	//FTransform IKHandGunTransform = Character->GetMesh()->GetSocketTransform(FName(TEXT("ik_hand_gun")));
	FTransform IKHandGunTransform = Character->GetArmMesh()->GetSocketTransform(FName(TEXT("ik_hand_gun")));

	AimSocketRelativeTransform = AimSocketTransform.GetRelativeTransform(IKHandGunTransform);
}

void AWeapon::SetAimSocketRelativeTransform()
{
	AimSocketRelativeTransform = WeaponMesh->GetSocketTransform(FName(TEXT("Aim")), ERelativeTransformSpace::RTS_Component);
}

FTransform AWeapon::GetAimSocketRelativeTransform()
{
	return AimSocketRelativeTransform;
}

#pragma region Equip/Unequip
void AWeapon::SwitchWeapon(ASuraPawnPlayer* TargetCharacter, bool bEquip)
{
	//TODO: Targeting ���̶�� ���� ��ü �Ұ��� �ؾ���

	if (CurrentState == ReloadingState || CurrentState == PumpActionReloadingState)
	{
		CancelReload();
	}
	else if (CurrentState == FiringState)
	{
		BurstShotFired = 0;
		GetWorld()->GetTimerManager().ClearTimer(SingleShotTimer);
		GetWorld()->GetTimerManager().ClearTimer(FullAutoShotTimer);
		GetWorld()->GetTimerManager().ClearTimer(BurstShotTimer);
	}

	ChangeState(SwitchingState);

	if (bEquip)
	{
		AttachWeaponToPlayer(TargetCharacter);
		GetWorld()->GetTimerManager().SetTimer(SwitchingTimer, [this, TargetCharacter, bEquip]() {EndWeaponSwitch(TargetCharacter, bEquip); }, WeaponSwitchingRate, false);
		StartAnimation(AM_Equip_Character, nullptr, WeaponSwitchingRate, WeaponSwitchingRate);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(SwitchingTimer, [this, TargetCharacter, bEquip]() {EndWeaponSwitch(TargetCharacter, bEquip); }, WeaponSwitchingRate, false);
		StartAnimation(AM_Unequip_Character, nullptr, WeaponSwitchingRate, WeaponSwitchingRate);

		AM_Unequip_Character->BlendOut.SetBlendTime(1000.f);
		AM_Unequip_Character->bEnableAutoBlendOut = false;
	}
}
void AWeapon::EndWeaponSwitch(ASuraPawnPlayer* TargetCharacter, bool bEquip)
{
	if (!TargetCharacter) { return; }

	if (bEquip) { EquipWeapon(TargetCharacter); }
	else
	{
		UnequipWeapon(TargetCharacter);
		if (UActorComponent* WeaponSystem = TargetCharacter->GetWeaponSystemComponent())
		{
			if (IWeaponInterface* WeaponInterface = Cast<IWeaponInterface>(WeaponSystem))
			{
				WeaponInterface->SwitchToOtherWeapon();
			}
		}
	}
}
void AWeapon::EquipWeapon(ASuraPawnPlayer* TargetCharacter, bool bActivateDirectly)
{
	SetInputActionBinding();
	ChangeState(IdleState);

	if (bActivateDirectly)
	{
		AttachWeaponToPlayer(TargetCharacter);
		//SetMeshVisibility(true);
		//ActivateTargetingSkillWidget(true);
	}
}

void AWeapon::UnequipWeapon(ASuraPawnPlayer* TargetCharacter)
{
	ResetInputActionBinding();
	DetachWeaponFromPlayer();
	ChangeState(UnequippedState);
}

void AWeapon::SetInputActionBinding()
{
	if (Character)
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
		{
			if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
			{
				// <LeftMouseAction>
				if (LeftMouseAction == EWeaponAction::WeaponAction_SingleShot)
				{
					InputActionBindingHandles.Add(&EnhancedInputComponent->BindActionValueLambda(
						LeftSingleShotAction,
						ETriggerEvent::Started,
						[this](const FInputActionValue& InputActionValue, bool bIsLeftInput, bool bSingleProjectile, int32 NumPenetrable)
						{
							HandleSingleFire(bIsLeftInput, bSingleProjectile, NumPenetrable);
						},
						true, !bEnableMultiProjectile_L, MaxPenetrableObjectsNum_Left
					));
				}
				else if (LeftMouseAction == EWeaponAction::WeaponAction_BurstShot)
				{
					InputActionBindingHandles.Add(&EnhancedInputComponent->BindActionValueLambda(
						LeftBurstShotAction,
						ETriggerEvent::Started,
						[this](const FInputActionValue& InputActionValue, bool bIsLeftInput, bool bSingleProjectile, int32 NumPenetrable)
						{
							HandleBurstFire(bIsLeftInput, bSingleProjectile, NumPenetrable);
						},
						true, !bEnableMultiProjectile_L, MaxPenetrableObjectsNum_Left
					));
				}
				else if (LeftMouseAction == EWeaponAction::WeaponAction_FullAutoShot)
				{
					InputActionBindingHandles.Add(&EnhancedInputComponent->BindActionValueLambda(
						LeftFullAutoShotAction,
						ETriggerEvent::Triggered,
						[this](const FInputActionValue& InputActionValue, bool bIsLeftInput, bool bSingleProjectile, int32 NumPenetrable)
						{
							StartFullAutoShot(bIsLeftInput, bSingleProjectile, NumPenetrable);
						},
						true, !bEnableMultiProjectile_L, MaxPenetrableObjectsNum_Left
					));
					InputActionBindingHandles.Add(&EnhancedInputComponent->BindAction(LeftFullAutoShotAction, ETriggerEvent::Completed, this, &AWeapon::StopFullAutoShot));
				}
				else if (LeftMouseAction == EWeaponAction::WeaponAction_Charge)
				{
					InputActionBindingHandles.Add(&EnhancedInputComponent->BindAction(LeftChargeAction, ETriggerEvent::Triggered, this, &AWeapon::StartCharge));
					InputActionBindingHandles.Add(&EnhancedInputComponent->BindAction(LeftChargeAction, ETriggerEvent::Completed, this, &AWeapon::StopCharge));
				}
				else if (LeftMouseAction == EWeaponAction::WeaponAction_Hold)
				{
					InputActionBindingHandles.Add(&EnhancedInputComponent->BindAction(LeftHoldAction, ETriggerEvent::Triggered, this, &AWeapon::StartTargetDetection));
					InputActionBindingHandles.Add(&EnhancedInputComponent->BindActionValueLambda(
						LeftHoldAction,
						ETriggerEvent::Completed,
						[this](const FInputActionValue& InputActionValue, FWeaponFireData* InFireData)
						{
							StopTargetDetection(InFireData);
						},
						&FireData_L
					));
				}
				else if (LeftMouseAction == EWeaponAction::WeaponAction_Zoom)
				{
					// No Left Zoom
				}
				else if (LeftMouseAction == EWeaponAction::WeaponAction_SkillFire)
				{
					InputActionBindingHandles.Add(&EnhancedInputComponent->BindActionValueLambda(
						LeftSingleShotAction,
						ETriggerEvent::Started,
						[this](const FInputActionValue& InputActionValue, bool bIsLeftInput, bool bSingleProjectile, int32 NumPenetrable)
						{
							HandleTargetingSkillFire(bIsLeftInput, bSingleProjectile, NumPenetrable);
						},
						true, !bEnableMultiProjectile_L, MaxPenetrableObjectsNum_Left
					));
				}

				// <RightMouseAction>
				if (RightMouseAction == EWeaponAction::WeaponAction_SingleShot)
				{
					InputActionBindingHandles.Add(&EnhancedInputComponent->BindActionValueLambda(
						RightSingleShotAction,
						ETriggerEvent::Started,
						[this](const FInputActionValue& InputActionValue, bool bIsLeftInput, bool bSingleProjectile, int32 NumPenetrable)
						{
							HandleSingleFire(bIsLeftInput, bSingleProjectile, NumPenetrable);
						},
						false, !bEnableMultiProjectile_R, MaxPenetrableObjectsNum_Right
					));
				}
				else if (RightMouseAction == EWeaponAction::WeaponAction_BurstShot)
				{
					InputActionBindingHandles.Add(&EnhancedInputComponent->BindActionValueLambda(
						RightBurstShotAction,
						ETriggerEvent::Started,
						[this](const FInputActionValue& InputActionValue, bool bIsLeftInput, bool bSingleProjectile, int32 NumPenetrable)
						{
							HandleBurstFire(bIsLeftInput, bSingleProjectile, NumPenetrable);
						},
						false, !bEnableMultiProjectile_R, MaxPenetrableObjectsNum_Right
					));
				}
				else if (RightMouseAction == EWeaponAction::WeaponAction_FullAutoShot)
				{
					InputActionBindingHandles.Add(&EnhancedInputComponent->BindActionValueLambda(
						RightFullAutoShotAction,
						ETriggerEvent::Started,
						[this](const FInputActionValue& InputActionValue, bool bIsLeftInput, bool bSingleProjectile, int32 NumPenetrable)
						{
							StartFullAutoShot(bIsLeftInput, bSingleProjectile, NumPenetrable);
						},
						false, !bEnableMultiProjectile_R, MaxPenetrableObjectsNum_Right
					));
					InputActionBindingHandles.Add(&EnhancedInputComponent->BindAction(RightFullAutoShotAction, ETriggerEvent::Completed, this, &AWeapon::StopFullAutoShot));
				}
				else if (RightMouseAction == EWeaponAction::WeaponAction_Charge)
				{
					InputActionBindingHandles.Add(&EnhancedInputComponent->BindAction(RightChargeAction, ETriggerEvent::Triggered, this, &AWeapon::StartCharge));
					InputActionBindingHandles.Add(&EnhancedInputComponent->BindAction(RightChargeAction, ETriggerEvent::Completed, this, &AWeapon::StopCharge));
				}
				else if (RightMouseAction == EWeaponAction::WeaponAction_Hold)
				{
					InputActionBindingHandles.Add(&EnhancedInputComponent->BindAction(RightHoldAction, ETriggerEvent::Triggered, this, &AWeapon::StartTargetDetection));
					InputActionBindingHandles.Add(&EnhancedInputComponent->BindActionValueLambda(
						RightHoldAction,
						ETriggerEvent::Completed,
						[this](const FInputActionValue& InputActionValue, FWeaponFireData* InFireData)
						{
							StopTargetDetection(InFireData);
						},
						&FireData_R
					));
				}
				else if (RightMouseAction == EWeaponAction::WeaponAction_Zoom)
				{
					InputActionBindingHandles.Add(&EnhancedInputComponent->BindAction(RightZoomAction, ETriggerEvent::Started, this, &AWeapon::ZoomToggle));
				}
				else if (RightMouseAction == EWeaponAction::WeaponAction_SkillFire)
				{
					InputActionBindingHandles.Add(&EnhancedInputComponent->BindActionValueLambda(
						RightSingleShotAction,
						ETriggerEvent::Started,
						[this](const FInputActionValue& InputActionValue, bool bIsLeftInput, bool bSingleProjectile, int32 NumPenetrable)
						{
							HandleTargetingSkillFire(bIsLeftInput, bSingleProjectile, NumPenetrable);
						},
						false, !bEnableMultiProjectile_R, MaxPenetrableObjectsNum_Right
					));
				}

				// <Skill>
				if (SkillAction == EWeaponAction::WeaponAction_SkillToggle)
				{
					InputActionBindingHandles.Add(&EnhancedInputComponent->BindAction(SkillToggleAction, ETriggerEvent::Started, this, &AWeapon::HandleTargetDetectionSkill));
				}

				// <Reload>
				if (bActivePumpActionReload)
				{
					InputActionBindingHandles.Add(&EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &AWeapon::HandlePumpActionReload));
				}
				else
				{
					InputActionBindingHandles.Add(&EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &AWeapon::HandleReload));
				}
			}
		}
	}
}
void AWeapon::ResetInputActionBinding()
{
	if (Character)
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
		{
			if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
			{
				for (FInputBindingHandle* bindinghandle : InputActionBindingHandles)
				{
					EnhancedInputComponent->RemoveBinding(*bindinghandle);
				}
				InputActionBindingHandles.Empty();
			}
		}
	}
}
#pragma endregion

#pragma region Reload
void AWeapon::HandleReload()
{
	if (CurrentState == IdleState)
	{
		if (LeftAmmoInCurrentMag < MaxAmmoPerMag && TotalAmmo > 0)
		{
			//TODO: ReloadingState�� EnterState���� StartReload �ص� �� ��
			ChangeState(ReloadingState);
			//StartReload();
		}
	}
}
void AWeapon::HandlePumpActionReload()
{
	if (CurrentState == IdleState)
	{
		if (LeftAmmoInCurrentMag < MaxAmmoPerMag && TotalAmmo > 0)
		{
			ChangeState(PumpActionReloadingState);
		}
	}
}
void AWeapon::CancelReload()
{
	CancelAnimation(AM_Reload_Character, AM_Reload_Weapon);
	GetWorld()->GetTimerManager().ClearTimer(ReloadingTimer);
}
void AWeapon::StartReload()
{
	UE_LOG(LogTemp, Warning, TEXT("Start Reload!!!"));

	if (bIsZoomIn)
	{
		ZoomOut();
	}

	// TODO: ���⼭ �Ϲ� Reloading���� OverHitting Recover���� �����ؼ� �ٸ� ó���� �������
	StartAnimation(AM_Reload_Character, AM_Reload_Weapon, ReloadingTime, ReloadingTime);
	GetWorld()->GetTimerManager().SetTimer(ReloadingTimer, this, &AWeapon::StopReload, ReloadingTime, false);
}
void AWeapon::StartPumpActionReload(bool bStartFromMiddle)
{
	if (bIsZoomIn)
	{
		ZoomOut();
	}

	if (bStartFromMiddle)
	{
		if (LeftAmmoInCurrentMag + 1 == MaxAmmoPerMag)
		{
			if (CharacterAnimInstance != nullptr && AM_Reload_Character != nullptr)
			{
				float SectionTime = AM_Reload_Character->GetSectionLength(AM_Reload_Character->GetSectionIndex(FName("LoopEnd")));

				UE_LOG(LogTemp, Error, TEXT("SectionTime_End: %f"), SectionTime);
				UE_LOG(LogTemp, Error, TEXT("PumpReloadingTime_End: %f"), PumpReloadingTime_End);

				CharacterAnimInstance->Montage_Play(AM_Reload_Character, SectionTime / PumpReloadingTime_End);
				CharacterAnimInstance->Montage_JumpToSection(FName("LoopEnd"), AM_Reload_Character);
			}

			if (WeaponAnimInstance != nullptr && AM_Reload_Weapon != nullptr)
			{
				WeaponMesh->GetAnimInstance()->Montage_Play(AM_Reload_Weapon, AM_Reload_Weapon->GetPlayLength() / PumpReloadingTime_End);
				WeaponMesh->GetAnimInstance()->Montage_JumpToSection(FName("LoopEnd"), AM_Reload_Weapon);
			}

			GetWorld()->GetTimerManager().SetTimer(ReloadingTimer, this, &AWeapon::StopPumpActionReload, PumpReloadingTime_End, false);
		}
		else
		{
			if (CharacterAnimInstance != nullptr && AM_Reload_Character != nullptr)
			{
				float SectionTime = AM_Reload_Character->GetSectionLength(AM_Reload_Character->GetSectionIndex(FName("LoopStart")));

				UE_LOG(LogTemp, Error, TEXT("SectionTime_Loop: %f"), SectionTime);
				UE_LOG(LogTemp, Error, TEXT("PumpReloadingTime_Loop: %f"), PumpReloadingTime_Loop);

				CharacterAnimInstance->Montage_Play(AM_Reload_Character, SectionTime / PumpReloadingTime_Loop);
				CharacterAnimInstance->Montage_JumpToSection(FName("LoopStart"), AM_Reload_Character);

				AM_Reload_Character->BlendIn.SetBlendTime(0.f);
				AM_Reload_Character->bEnableAutoBlendOut = false;
			}

			if (WeaponAnimInstance != nullptr && AM_Reload_Weapon != nullptr)
			{
				WeaponMesh->GetAnimInstance()->Montage_Play(AM_Reload_Weapon, AM_Reload_Weapon->GetPlayLength() / PumpReloadingTime_Loop);
				WeaponMesh->GetAnimInstance()->Montage_JumpToSection(FName("LoopStart"), AM_Reload_Weapon);
			}

			GetWorld()->GetTimerManager().SetTimer(ReloadingTimer, this, &AWeapon::StopPumpActionReload, PumpReloadingTime_Loop, false);
		}
	}
	else
	{
		if (CharacterAnimInstance->Montage_IsPlaying(AM_Reload_Weapon))
		{
			CharacterAnimInstance->Montage_Stop(0.f, AM_Reload_Weapon);
		}

		if (CharacterAnimInstance != nullptr && AM_Reload_Character != nullptr)
		{
			float SectionTime = AM_Reload_Character->GetSectionLength(AM_Reload_Character->GetSectionIndex(FName("Start")));

			UE_LOG(LogTemp, Error, TEXT("SectionTime_Start: %f"), SectionTime);
			UE_LOG(LogTemp, Error, TEXT("PumpReloadingTime_Start: %f"), PumpReloadingTime_Start);

			CharacterAnimInstance->Montage_Play(AM_Reload_Character, SectionTime / PumpReloadingTime_Start);
			CharacterAnimInstance->Montage_JumpToSection(FName("Start"), AM_Reload_Character);

			AM_Reload_Character->BlendOut.SetBlendTime(1000.f);
			AM_Reload_Character->bEnableAutoBlendOut = false;
		}

		if (WeaponAnimInstance != nullptr && AM_Reload_Weapon != nullptr)
		{
			WeaponMesh->GetAnimInstance()->Montage_Play(AM_Reload_Weapon, AM_Reload_Weapon->GetPlayLength() / PumpReloadingTime_Start);
			WeaponMesh->GetAnimInstance()->Montage_JumpToSection(FName("Start"), AM_Reload_Weapon);
		}

		GetWorld()->GetTimerManager().SetTimer(ReloadingTimer, this, &AWeapon::StopPumpActionReload, PumpReloadingTime_Start, false);
	}
}
void AWeapon::StopReload()
{
	ReloadAmmo();
	ChangeState(IdleState);
}

void AWeapon::StopPumpActionReload()
{
	//ReloadAmmo(true);

	if (BufferedFireRequest.IsSet())
	{
		if (LeftAmmoInCurrentMag >= MaxAmmoPerMag)
		{
			ChangeState(IdleState);

			const FBufferedFireRequest& Request = BufferedFireRequest.GetValue();
			if (Request.ActionName == EWeaponAction::WeaponAction_SingleShot)
			{
				HandleSingleFire(Request.bIsLeftInput, Request.bSingleProjectile, Request.NumPenetrable);
				BufferedFireRequest.Reset();
				//UE_LOG(LogTemp, Log, TEXT("Buffered fire executed after reload"));
			}
			else
			{
				HandleBurstFire(Request.bIsLeftInput, Request.bSingleProjectile, Request.NumPenetrable);
				BufferedFireRequest.Reset();
				//UE_LOG(LogTemp, Log, TEXT("Buffered fire executed after reload"));
			}
		}
		else
		{
			float SectionTime = AM_Reload_Character->GetSectionLength(AM_Reload_Character->GetSectionIndex(FName("End")));

			CharacterAnimInstance->Montage_Play(AM_Reload_Character, SectionTime / PumpReloadingTime_LoopToFire);
			CharacterAnimInstance->Montage_JumpToSection(FName("End"), AM_Reload_Character);

			GetWorld()->GetTimerManager().SetTimer(ReloadingTimer, this, &AWeapon::InterruptReloadAndFire, PumpReloadingTime_LoopToFire, false);
		}

	}
	else if (bFireInputDuringReload)
	{
		if (LeftAmmoInCurrentMag >= MaxAmmoPerMag)
		{
			bFireInputDuringReload = false;
			ChangeState(IdleState);
		}
		else
		{
			float SectionTime = AM_Reload_Character->GetSectionLength(AM_Reload_Character->GetSectionIndex(FName("End")));

			CharacterAnimInstance->Montage_Play(AM_Reload_Character, SectionTime / PumpReloadingTime_LoopToFire);
			CharacterAnimInstance->Montage_JumpToSection(FName("End"), AM_Reload_Character);

			GetWorld()->GetTimerManager().SetTimer(ReloadingTimer, this, &AWeapon::InterruptReloadAndFire, PumpReloadingTime_LoopToFire, false);
		}
	}
	//if (BufferedFireRequest.IsSet() || bFireInputDuringReload)
	//{
	//	float SectionTime = AM_Reload_Character->GetSectionLength(AM_Reload_Character->GetSectionIndex(FName("End")));

	//	CharacterAnimInstance->Montage_Play(AM_Reload_Character, SectionTime / PumpReloadingTime_LoopToFire);
	//	CharacterAnimInstance->Montage_JumpToSection(FName("End"), AM_Reload_Character);

	//	GetWorld()->GetTimerManager().SetTimer(ReloadingTimer, this, &AWeapon::InterruptReloadAndFire, PumpReloadingTime_LoopToFire, false);
	//}
	else
	{
		if (LeftAmmoInCurrentMag < MaxAmmoPerMag)
		{
			StartPumpActionReload(true);
		}
		else
		{
			ChangeState(IdleState);
		}
	}
}

void AWeapon::InterruptReloadAndFire()
{
	if (BufferedFireRequest.IsSet())
	{
		ChangeState(IdleState);

		const FBufferedFireRequest& Request = BufferedFireRequest.GetValue();
		if (Request.ActionName == EWeaponAction::WeaponAction_SingleShot)
		{
			HandleSingleFire(Request.bIsLeftInput, Request.bSingleProjectile, Request.NumPenetrable);
			BufferedFireRequest.Reset();
			//UE_LOG(LogTemp, Log, TEXT("Buffered fire executed after reload"));
		}
		else
		{
			HandleBurstFire(Request.bIsLeftInput, Request.bSingleProjectile, Request.NumPenetrable);
			BufferedFireRequest.Reset();
			//UE_LOG(LogTemp, Log, TEXT("Buffered fire executed after reload"));
		}
	}
	else if (bFireInputDuringReload)
	{
		UE_LOG(LogTemp, Error, TEXT("Why1?"));
		bFireInputDuringReload = false;
		ChangeState(IdleState);
	}
}

void AWeapon::ConsumeAmmo(int32 AmmoCost, bool AllowFireWithInsufficientAmmo)
{
	if (LeftAmmoInCurrentMag > 0)
	{
		if (AllowFireWithInsufficientAmmo)
		{
			if (LeftAmmoInCurrentMag >= AmmoCost)
			{
				LeftAmmoInCurrentMag -= AmmoCost;
			}
			else
			{
				LeftAmmoInCurrentMag = 0;
			}
		}
		else
		{
			LeftAmmoInCurrentMag -= AmmoCost;
		}

		if (AmmoCounterWidget)
		{
			AmmoCounterWidget->UpdateAmmoCount(LeftAmmoInCurrentMag);
		}
	}
}
void AWeapon::ReloadAmmo(bool bPumpAction)
{
	if (bPumpAction)
	{
		int32 RequiredAmmo = 1;
		int32 AmmoToReload = FMath::Min(RequiredAmmo, TotalAmmo);

		LeftAmmoInCurrentMag += AmmoToReload;
		TotalAmmo -= AmmoToReload;

		if (AmmoCounterWidget)
		{
			AmmoCounterWidget->UpdateAmmoCount(LeftAmmoInCurrentMag);
			AmmoCounterWidget->UpdateTotalAmmo(TotalAmmo);
		}
	}
	else
	{
		//int32 RequiredAmmo = MaxAmmoPerMag - LeftAmmoInCurrentMag;
		//if (TotalAmmo < RequiredAmmo)
		//{
		//	LeftAmmoInCurrentMag = LeftAmmoInCurrentMag + TotalAmmo;
		//	TotalAmmo = 0;
		//}
		//else
		//{
		//	TotalAmmo = TotalAmmo - RequiredAmmo;
		//	LeftAmmoInCurrentMag = MaxAmmoPerMag;
		//}

		int32 RequiredAmmo = MaxAmmoPerMag - LeftAmmoInCurrentMag;
		int32 AmmoToReload = FMath::Min(RequiredAmmo, TotalAmmo);

		LeftAmmoInCurrentMag += AmmoToReload;
		TotalAmmo -= AmmoToReload;

		//--------------

		//LeftAmmoInCurrentMag = MaxAmmoPerMag;
		if (AmmoCounterWidget)
		{
			AmmoCounterWidget->UpdateAmmoCount(LeftAmmoInCurrentMag);
			AmmoCounterWidget->UpdateTotalAmmo(TotalAmmo);
		}
	}
}
bool AWeapon::HasAmmoInCurrentMag()
{
	return (LeftAmmoInCurrentMag > 0);
}
bool AWeapon::HasAmmoInCurrentMag(int32 AmmoCost)
{
	return LeftAmmoInCurrentMag >= AmmoCost;
}
bool AWeapon::AddAmmo(int32 NumAmmo)
{
	if (TotalAmmo < MaxTotalAmmo)
	{
		int32 NewTotalAmmo = FMath::Clamp(TotalAmmo + NumAmmo, 0, MaxTotalAmmo);
		TotalAmmo = NewTotalAmmo;
		if (AmmoCounterWidget)
		{
			AmmoCounterWidget->UpdateAmmoCount(LeftAmmoInCurrentMag);
			AmmoCounterWidget->UpdateTotalAmmo(TotalAmmo);
		}
		return true;
	}
	return false;
}
void AWeapon::AutoReload()
{
	if (bCanAutoReload)
	{
		if (!HasAmmoInCurrentMag() && TotalAmmo > 0)
		{
			if (bActivePumpActionReload)
			{
				ChangeState(PumpActionReloadingState);
			}
			else
			{
				ChangeState(ReloadingState);
			}
		}
	}
}
void AWeapon::ReloadingEnd()
{
	UE_LOG(LogTemp, Warning, TEXT("Reloading End!!!"));
	ReloadAmmo(true);
}
#pragma endregion

#pragma region UI
void AWeapon::ActivateAimUIWidget(bool bflag)
{
	if (bflag)
	{
		if (AimUIWidget)
		{
			AimUIWidget->AddToViewport();
			AimUIWidget->ResetAimUISize();
		}
	}
	else
	{
		if (AimUIWidget)
		{
			AimUIWidget->RemoveFromViewport();
		}
	}
}
void AWeapon::ActivateAmmoCounterWidget(bool bflag)
{
	if (bflag)
	{
		if (AmmoCounterWidget)
		{
			AmmoCounterWidget->AddToViewport();
		}
	}
	else
	{
		if (AmmoCounterWidget)
		{
			AmmoCounterWidget->RemoveFromViewport();
		}
	}
}
void AWeapon::ActivateTargetingSkillWidget(bool bflag)
{
	if (bflag)
	{
		if (TargetingSkillWidget)
		{
			TargetingSkillWidget->AddToViewport();
		}
	}
	else
	{
		if (TargetingSkillWidget)
		{
			TargetingSkillWidget->RemoveFromViewport();
		}
	}
}
void AWeapon::SetUpAimUIDelegateBinding(ASuraProjectile* Projectile)
{
	if (AimUIWidget)
	{
		AimUIWidget->SetUpAimUIDelegateBinding(Projectile);
	}
}
#pragma endregion

#pragma region FireMode
void AWeapon::HandleSingleFire(bool bIsLeftInput, bool bSingleProjectile, int32 NumPenetrable)
{
	//UE_LOG(LogTemp, Warning, TEXT("Handle Single Fire"));
	if (CurrentState == IdleState)
	{
		ChangeState(FiringState);
		StartSingleShot(bIsLeftInput, bSingleProjectile, NumPenetrable);
	}
	else if (CurrentState == PumpActionReloadingState)
	{
		UE_LOG(LogTemp, Error, TEXT("Fire input buffered during reload"));
		BufferedFireRequest = FBufferedFireRequest(EWeaponAction::WeaponAction_SingleShot, bIsLeftInput, bSingleProjectile, NumPenetrable);
	}
}
void AWeapon::HandleBurstFire(bool bIsLeftInput, bool bSingleProjectile, int32 NumPenetrable)
{
	if (CurrentState == IdleState)
	{
		ChangeState(FiringState);
		StartBurstFire(bIsLeftInput, bSingleProjectile, NumPenetrable);
	}
	else if (CurrentState == PumpActionReloadingState)
	{
		UE_LOG(LogTemp, Error, TEXT("Fire input buffered during reload"));
		BufferedFireRequest = FBufferedFireRequest(EWeaponAction::WeaponAction_BurstShot, bIsLeftInput, bSingleProjectile, NumPenetrable);
	}
}
void AWeapon::HandleFullAutoFire() //TODO: �Ⱦ���. ���� ���
{
	if (CurrentState == IdleState)
	{
		ChangeState(FiringState);
		StartSingleShot();
	}
}
#pragma endregion

#pragma region FireMode/SingleShot
void AWeapon::StartSingleShot(bool bIsLeftInput, bool bSingleProjectile, int32 NumPenetrable, float AdditionalDamage, float AdditionalRecoilAmountPitch, float AdditionalRecoilAmountYaw, float AdditionalProjectileRadius, int32 AdditionalPellet)
{
	if (bIsLeftInput)
	{
		if (bSingleProjectile)
		{
			if (bIsHitScan_L) { FireSingleHitScan(&FireData_L, NumPenetrable, AdditionalDamage, AdditionalRecoilAmountPitch, AdditionalRecoilAmountYaw, AdditionalProjectileRadius); }
			else { FireSingleProjectile(&FireData_L, NumPenetrable, AdditionalDamage, AdditionalRecoilAmountPitch, AdditionalRecoilAmountYaw, AdditionalProjectileRadius, false); }
		}
		else
		{ 
			FireMultiProjectile(&FireData_L, NumPenetrable, AdditionalDamage, AdditionalRecoilAmountPitch, AdditionalRecoilAmountYaw, AdditionalProjectileRadius, AdditionalPellet, false);
		}
	}
	else
	{
		if (bSingleProjectile)
		{
			if (bIsHitScan_R) { FireSingleHitScan(&FireData_R, NumPenetrable, AdditionalDamage, AdditionalRecoilAmountPitch, AdditionalRecoilAmountYaw, AdditionalProjectileRadius); }
			else { FireSingleProjectile(&FireData_R, NumPenetrable, AdditionalDamage, AdditionalRecoilAmountPitch, AdditionalRecoilAmountYaw, AdditionalProjectileRadius, false); }
		}
		else
		{
			FireMultiProjectile(&FireData_R, NumPenetrable, AdditionalDamage, AdditionalRecoilAmountPitch, AdditionalRecoilAmountYaw, AdditionalProjectileRadius, AdditionalPellet, false);
		}
	}

	GetWorld()->GetTimerManager().SetTimer(SingleShotTimer, this, &AWeapon::StopSingleShot, SingleShotDelay, false);
}
void AWeapon::StopSingleShot()
{
	ChangeState(IdleState);
}
#pragma endregion

#pragma region FireMode/BurstShot
void AWeapon::StartBurstFire(bool bIsLeftInput, bool bSingleProjectile, int32 NumPenetrable, float AdditionalDamage, float AdditionalRecoilAmountPitch, float AdditionalRecoilAmountYaw, float AdditionalProjectileRadius)
{
	if (BurstShotFired < BurstShotCount)
	{
		if (bIsLeftInput)
		{
			if (bSingleProjectile)
			{
				if (bIsHitScan_L) { FireSingleHitScan(&FireData_L, NumPenetrable,  AdditionalDamage, AdditionalRecoilAmountPitch, AdditionalRecoilAmountYaw); }
				else { FireSingleProjectile(&FireData_L, NumPenetrable, AdditionalDamage, AdditionalRecoilAmountPitch, AdditionalRecoilAmountYaw, false); }
			}
			else
			{
				FireMultiProjectile(&FireData_L, NumPenetrable, AdditionalDamage, AdditionalRecoilAmountPitch, AdditionalRecoilAmountYaw, 0, false);
			}
		}
		else
		{
			if (bSingleProjectile)
			{
				if (bIsHitScan_R) { FireSingleHitScan(&FireData_R, NumPenetrable, AdditionalDamage, AdditionalRecoilAmountPitch, AdditionalRecoilAmountYaw); }
				else { FireSingleProjectile(&FireData_R, NumPenetrable, AdditionalDamage, AdditionalRecoilAmountPitch, AdditionalRecoilAmountYaw, false); }
			}
			else
			{
				FireMultiProjectile(&FireData_R, NumPenetrable, AdditionalDamage, AdditionalRecoilAmountPitch, AdditionalRecoilAmountYaw, 0, false);
			}
		}
		BurstShotFired++;
		GetWorld()->GetTimerManager().SetTimer(BurstShotTimer, [this, bIsLeftInput, bSingleProjectile, NumPenetrable]() {StartBurstFire(bIsLeftInput, bSingleProjectile, NumPenetrable); }, BurstShotFireRate, true);
	}
	else
	{
		StopBurstFire();
	}
}
void AWeapon::StopBurstFire()
{
	BurstShotFired = 0;
	if (GetWorld()->GetTimerManager().IsTimerActive(BurstShotTimer))
	{
		GetWorld()->GetTimerManager().ClearTimer(BurstShotTimer);
	}
	GetWorld()->GetTimerManager().SetTimer(BurstShotTimer, this, &AWeapon::EndBurstShot, BurstShotDelay, false);
}
void AWeapon::EndBurstShot()
{
	ChangeState(IdleState);
}
#pragma endregion

#pragma region FireMode/FullAuto
void AWeapon::StartFullAutoShot(bool bIsLeftInput, bool bSingleProjectile, int32 NumPenetrable)
{
	if (CurrentState == IdleState)
	{
		ChangeState(FiringState);
		UpdateFullAutoShot(bIsLeftInput, bSingleProjectile, NumPenetrable);
	}
	else if (CurrentState == PumpActionReloadingState)
	{
		bFireInputDuringReload = true;
	}
}
void AWeapon::UpdateFullAutoShot(bool bIsLeftInput, bool bSingleProjectile, int32 NumPenetrable)
{
	if (bSingleProjectile)
	{
		if (bIsLeftInput)
		{
			if (bIsHitScan_L) { FireSingleHitScan(&FireData_L, NumPenetrable); }
			else { FireSingleProjectile(&FireData_L, NumPenetrable); }
		}
		else
		{
			if (bIsHitScan_R) { FireSingleHitScan(&FireData_R, NumPenetrable); }
			else { FireSingleProjectile(&FireData_R, NumPenetrable); }
		}
	}
	else
	{
		if (bIsLeftInput)
		{
			FireMultiProjectile(&FireData_L, NumPenetrable);
		}
		else
		{
			FireMultiProjectile(&FireData_R, NumPenetrable);
		}	
	}

	if (HasAmmoInCurrentMag())
	{
		GetWorld()->GetTimerManager().SetTimer(FullAutoShotTimer, [this, bIsLeftInput, bSingleProjectile, NumPenetrable]() {UpdateFullAutoShot(bIsLeftInput, bSingleProjectile, NumPenetrable); }, FullAutoShotFireRate, false);
	}
	else
	{
		StopFullAutoShot();
	}
}
void AWeapon::StopFullAutoShot()
{
	if (CurrentState == FiringState)
	{
		//UE_LOG(LogTemp, Warning, TEXT("FullAutoShot Ended!!!"));
		GetWorld()->GetTimerManager().ClearTimer(FullAutoShotTimer);
		ChangeState(IdleState);
	}
	else if (CurrentState == PumpActionReloadingState)
	{
		bFireInputDuringReload = false;
	}
}
#pragma endregion

#pragma region FireMode/Targeting
void AWeapon::StartTargetDetection()
{
	if (CurrentState == IdleState)
	{
		UE_LOG(LogTemp, Warning, TEXT("Start Target Detection!!!"));

		ChangeState(TargetingState);
		PlayWeaponSound(TargetSearchLoopSound);
		UpdateTargetDetection(GetWorld()->GetDeltaSeconds());
	}
}
void AWeapon::UpdateTargetDetection(float DeltaTime) //TODO: �ش� Ÿ�� Ȥ�� ���� Ÿ���� ���� ���ε� �Ǵ��ؼ� Update �ؾ���
{
	ElapsedTimeAfterTargetingStarted += DeltaTime;

	TArray<AActor*> NewOverlappedActors;

	CurrentTargetDetectionRadius = (FMath::Clamp(ElapsedTimeAfterTargetingStarted, 0.f, TimeToReachMaxTargetDetectionRange) / TimeToReachMaxTargetDetectionRange) * MaxTargetDetectionRadius;
	CurrentTargetDetectionAngle = (FMath::Clamp(ElapsedTimeAfterTargetingStarted, 0.f, TimeToReachMaxTargetDetectionRange) / TimeToReachMaxTargetDetectionRange) * MaxTargetDetectionAngle;

	SearchOverlappedActor(Character->GetActorLocation(), CurrentTargetDetectionRadius, NewOverlappedActors);


	//TODO: Targets�� ���� Update�� �ʿ���. �׾����� Targets���� ���ܽ��Ѿ���
	// TargetMarker�� ��쿡�� Visibility�� false�� �ٲ��ִ� ������ �����ϱ�

	for (TSet<AActor*>::TIterator It = Targets.CreateIterator(); It; ++It)
	{
		AActor* PreviousTarget = *It;
		if (IsValid(PreviousTarget) && CheckIfTargetIsBlockedByObstacle(PreviousTarget))
		{
			UTargetMarkerWidget** TargetMarkerPtr = MapTargetActorToWidget.Find(PreviousTarget);
			(*TargetMarkerPtr)->RemoveFromViewport();
			(*TargetMarkerPtr)->RemoveFromParent();
			TargetMarkerWidgets.Remove(*TargetMarkerPtr);

			It.RemoveCurrent();
			MapTargetActorToWidget.Remove(PreviousTarget);
		}
	}

	for (AActor* NewOverlappedActor : NewOverlappedActors)
	{
		if (Targets.Num() >= MaxTargetNum || ElapsedTimeAfterTargetingStarted > MaxTargetDetectionTime)
		{
			break;
		}
		if (!Targets.Contains(NewOverlappedActor))
		{
			if (IsInViewport(GetScreenPositionOfWorldLocation(NewOverlappedActor->GetActorLocation()).Get<0>(), 1.f, 1.f))
			{
				if (GetUnsignedAngleBetweenVectors(Character->GetActorForwardVector(), NewOverlappedActor->GetActorLocation() - Character->GetActorLocation(), FVector::ZAxisVector) < CurrentTargetDetectionAngle)
				{
					if (!CheckIfTargetIsBlockedByObstacle(NewOverlappedActor))
					{
						Targets.Add(NewOverlappedActor);
						UTargetMarkerWidget* NewTargetMarker = CreateTargetMarkerWidget(NewOverlappedActor);
						if (NewTargetMarker)
						{
							TargetMarkerWidgets.Add(NewTargetMarker);
							NewTargetMarker->AddToViewport();
							NewTargetMarker->SetAlignmentInViewport(FVector2D(0.5f, 0.5f));
							NewTargetMarker->SetVisibility(ESlateVisibility::Hidden);
						}

						if (TargetLockedSound)
						{
							UGameplayStatics::PlaySoundAtLocation(this, TargetLockedSound, Character->GetActorLocation());
						}
					}
				}
			}
		}
	}


	float DeltaSeconds = GetWorld()->GetDeltaSeconds();
	GetWorld()->GetTimerManager().SetTimer(TargetDetectionTimer, [this, DeltaSeconds]() {UpdateTargetDetection(DeltaSeconds); }, DeltaSeconds, false);
}
void AWeapon::StopTargetDetection(FWeaponFireData* FireData)
{
	if (CurrentState == TargetingState)
	{
		UE_LOG(LogTemp, Warning, TEXT("Stop Target Detection!!!"));

		GetWorld()->GetTimerManager().ClearTimer(TargetDetectionTimer);

		ElapsedTimeAfterTargetingStarted = 0.f;
		CurrentTargetDetectionRadius = 0.f;
		CurrentTargetDetectionAngle = 0.f;

		ResetTargetMarkers();

		StopWeaponSound();

		TArray<AActor*> TargetsArray = Targets.Array();
		Targets.Empty();
		StartMissileLaunch(TargetsArray, FireData);
	}
}

bool AWeapon::SearchOverlappedActor(FVector CenterLocation, float SearchRadius, TArray<AActor*>& OverlappedActors)
{
	TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes;
	traceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	traceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel6));
	TArray<AActor*> ignoreActors;
	ignoreActors.Init(Character, 1);
	bool bIsAnyActorExist = UKismetSystemLibrary::SphereOverlapActors(GetWorld(), CenterLocation, SearchRadius, traceObjectTypes, nullptr, ignoreActors, OverlappedActors);

	return bIsAnyActorExist;
}
TTuple<FVector2D, bool> AWeapon::GetScreenPositionOfWorldLocation(const FVector& SearchLocation) const
{
	FVector2D ScreenLocation = FVector2D::ZeroVector;
	bool bResult = UGameplayStatics::ProjectWorldToScreen(CharacterController, SearchLocation, ScreenLocation);

	return MakeTuple(ScreenLocation, bResult);
}
bool AWeapon::IsInViewport(FVector2D ActorScreenPosition, float ScreenRatio_Width, float ScreenRatio_Height) const
{
	FVector2D ViewportSize = GEngine->GameViewport->Viewport->GetSizeXY();

	bool bIsInWidth = true;
	bool bIsInHeight = true;

	// Check Width
	if (ScreenRatio_Width == 0.0f || UKismetMathLibrary::Abs(ScreenRatio_Width) > 1.0f || (ScreenRatio_Width == (1.0f - ScreenRatio_Width)))
	{
		if (ActorScreenPosition.X >= 0.0f && ActorScreenPosition.X <= ViewportSize.X)
		{
			bIsInWidth = true;
		}
		else
		{
			bIsInWidth = false;
		}
	}
	else
	{
		float LargeScreenRatio_Width;
		float SmallScreenRatio_Width;

		if (ScreenRatio_Width < (1.0f - ScreenRatio_Width))
		{
			LargeScreenRatio_Width = 1.0f - ScreenRatio_Width;
			SmallScreenRatio_Width = ScreenRatio_Width;
		}
		else
		{
			LargeScreenRatio_Width = ScreenRatio_Width;
			SmallScreenRatio_Width = 1.0f - ScreenRatio_Width;
		}

		if (ActorScreenPosition.X >= ViewportSize.X * SmallScreenRatio_Width && ActorScreenPosition.X <= ViewportSize.X * LargeScreenRatio_Width)
		{
			bIsInWidth = true;
		}
		else
		{
			bIsInWidth = false;
		}
	}

	// Check Height
	if (ScreenRatio_Height == 0.0f || UKismetMathLibrary::Abs(ScreenRatio_Height) > 1.0f || (ScreenRatio_Height == (1.0f - ScreenRatio_Height)))
	{
		if (ActorScreenPosition.Y >= 0.0f && ActorScreenPosition.Y <= ViewportSize.Y)
		{
			bIsInHeight = true;
		}
		else
		{
			bIsInHeight = false;
		}
	}
	else
	{
		float LargeScreenRatio_Height;
		float SmallScreenRatio_Height;

		if (ScreenRatio_Height < (1.0f - ScreenRatio_Height))
		{
			LargeScreenRatio_Height = 1.0f - ScreenRatio_Height;
			SmallScreenRatio_Height = ScreenRatio_Height;
		}
		else
		{
			LargeScreenRatio_Height = ScreenRatio_Height;
			SmallScreenRatio_Height = 1.0f - ScreenRatio_Height;
		}

		if (ActorScreenPosition.Y >= ViewportSize.Y * SmallScreenRatio_Height && ActorScreenPosition.Y <= ViewportSize.Y * LargeScreenRatio_Height)
		{
			bIsInHeight = true;
		}
		else
		{
			bIsInHeight = false;
		}
	}

	// Return
	if (bIsInWidth && bIsInHeight)
	{
		return true;
	}
	else
	{
		return false;
	}
}
float AWeapon::GetUnsignedAngleBetweenVectors(const FVector& VectorA, const FVector& VectorB, const FVector& Axis)
{
	FVector NormalizedA = VectorA.GetSafeNormal();
	FVector NormalizedB = VectorB.GetSafeNormal();

	//�� ���� ������ ȸ���� ��Ÿ���� ���ʹϾ� ����
	FQuat Quat = FQuat::FindBetweenNormals(NormalizedA, NormalizedB);

	//ȸ�� �࿡ ���� ���⼺�� ����� ���� ���
	float Angle = Quat.GetAngle();  // �⺻������ ���� ������ ������ ��ȯ

	//���ʹϾ��� �࿡ �����Ͽ� ���� ����
	FVector RotationAxis = Quat.GetAxisZ();  // ���ʹϾ��� ȸ�� ���� ���� (���� X, Y, Z �� �ϳ��� ���� ����)

	float DotProduct = FVector::DotProduct(RotationAxis, Axis);

	// DotProduct�� ����� ������ �״��, ������ �ݴ� �������� ���� ��ȣ�� ����
	float SignedAngle = Angle * FMath::Sign(DotProduct);

	//������ �� ������ ��ȯ
	return FMath::RadiansToDegrees(Angle);
}
bool AWeapon::CheckIfTargetIsBlockedByObstacle(AActor* target)
{
	float Radius = 10.f;

	FVector StartPoint = Character->GetActorLocation();
	FVector EndPoint = target->GetActorLocation();

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));

	TArray<FHitResult> HitResults;
	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = false;
	QueryParams.bReturnPhysicalMaterial = false;
	QueryParams.AddIgnoredActor(Character);

	bool bHit = GetWorld()->SweepMultiByObjectType(
		HitResults,
		StartPoint,
		EndPoint,
		FQuat::Identity,
		FCollisionObjectQueryParams(ObjectTypes),
		FCollisionShape::MakeSphere(Radius),
		QueryParams
	);
	return bHit;
}

UTargetMarkerWidget* AWeapon::CreateTargetMarkerWidget(AActor* TargetActor)
{
	if (TargetMarkerWidgetClass)
	{
		UTargetMarkerWidget* NewTargetMarkerWidget = CreateWidget<UTargetMarkerWidget>(GetWorld(), TargetMarkerWidgetClass);
		MapTargetActorToWidget.Add(TargetActor, NewTargetMarkerWidget);
		
		return NewTargetMarkerWidget;
	}
	return nullptr;
}
void AWeapon::UpdateTargetMarkers()
{
	for (AActor* Target : Targets)
	{
		FVector TargetLocation = Target->GetActorLocation();
		//FVector TargetOffset(0.f, 0.f, 50.f);

		UTargetMarkerWidget** TargetMarkerPtr = MapTargetActorToWidget.Find(Target);

		USkeletalMeshComponent* TargetSkeletalMesh = Target->GetComponentByClass<USkeletalMeshComponent>();
		if (TargetSkeletalMesh && TargetSkeletalMesh->DoesSocketExist(FName(TEXT("spine_03"))))
		{
			TargetLocation = TargetSkeletalMesh->GetBoneLocation(FName(TEXT("spine_03")));
		}

		FVector2D TargetScreenPosition = GetScreenPositionOfWorldLocation(TargetLocation).Get<0>();

		if (IsInViewport(TargetScreenPosition, 1.f, 1.f))
		{
			(*TargetMarkerPtr)->SetPositionInViewport(TargetScreenPosition);

			if ((*TargetMarkerPtr)->Visibility == ESlateVisibility::Hidden)
			{
				(*TargetMarkerPtr)->SetVisibility(ESlateVisibility::Visible);
			}
		}
		else
		{
			if ((*TargetMarkerPtr)->Visibility == ESlateVisibility::Visible)
			{
				(*TargetMarkerPtr)->SetVisibility(ESlateVisibility::Hidden);
			}
		}
	}
}
void AWeapon::ResetTargetMarkers()
{
	for (AActor* Target : Targets)
	{
		UTargetMarkerWidget** TargetMarkerPtr = MapTargetActorToWidget.Find(Target);

		(*TargetMarkerPtr)->RemoveFromViewport();
		(*TargetMarkerPtr)->RemoveFromParent();
	}
	TargetMarkerWidgets.Empty();
	MapTargetActorToWidget.Empty();
}

void AWeapon::StartMissileLaunch(TArray<AActor*> TargetActors, FWeaponFireData* FireData)
{
	ConfirmedTargets = TargetActors;
	CurrentTargetIndex = 0;
	if (ConfirmedTargets.Num() == 0)
	{
		StopMissileLaunch();
	}
	else
	{
		ChangeState(FiringState);
		UpdateMissileLaunch(FireData);
	}
}
void AWeapon::UpdateMissileLaunch(FWeaponFireData* FireData)
{
	FireSingleProjectile(FireData, 0, 0.f, 0.f, 0.f, 0.f, true, ConfirmedTargets[CurrentTargetIndex]);
	CurrentTargetIndex++;
	if (ConfirmedTargets.Num() <= CurrentTargetIndex)
	{
		StopMissileLaunch();
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(MissileLaunchTimer, [this, FireData] {UpdateMissileLaunch(FireData); }, MissileLaunchDelay, false);
	}
}
void AWeapon::StopMissileLaunch()
{
	ConfirmedTargets.Empty();
	CurrentTargetIndex = 0;

	ReleaseControl();
	ChangeState(IdleState);
}
#pragma endregion

#pragma region FireMode/Charging
void AWeapon::StartCharge()
{
	if (CurrentState == IdleState)
	{
		ChangeState(ChargingState);

		SpawnChargeEffect(ChargeEffectLocation, ChargeEffectRotation, ChargeEffenctScale);
		PlayWeaponSound(ChargeSound);

		UpdateCharge();
	}
}
void AWeapon::UpdateCharge()
{
	float DeltaSeconds = GetWorld()->GetDeltaSeconds();
	ElapsedChargeTime += DeltaSeconds;

	float ChargingCamShakeScale = FMath::Clamp((ElapsedChargeTime / MaxChargeTime), 0.1f, 3.f); //TODO: Max�� ��������� �����ϴ��� �ؾ���
	ApplyCameraShake(ChargingCameraShakeClass, ChargingCamShakeScale);

	if (bAutoFireAtMaxChargeTime)
	{
		if (ElapsedChargeTime > MaxChargeTime)
		{
			//TODO: Timer�� ����� �ð��̶� ���� �ð��̶� �ٸ� �� ����. Log�� Ȯ���غ����� -> Tick ���� ����� �ð��̶� ���⼭ ����� �ð��̶� ���غ�����
			StopCharge();
		}
		else
		{
			GetWorld()->GetTimerManager().SetTimer(ChargingTimer, this, &AWeapon::UpdateCharge, DeltaSeconds, false);
		}
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(ChargingTimer, this, &AWeapon::UpdateCharge, DeltaSeconds, false);
	}
}
void AWeapon::StopCharge()
{
	if (CurrentState == ChargingState)
	{
		GetWorld()->GetTimerManager().ClearTimer(ChargingTimer);

		DestroyChargeEffect();
		StopWeaponSound();

		ChangeState(FiringState);

		float ChargingAdditionalDamage = 0.f;
		float AdditionalRecoilAmountPitch = 0.f;
		float AdditionalRecoilAmountYaw = 0.f;
		float AdditionalProjectileRadius = 0.f;
		int32 PenetrableObjectsNum = 0;
		int32 AdditionalPelletNum = 0;
		if (ElapsedChargeTime > ChargeTimeThreshold)
		{
			// TODO: Clamp �������...(���ص� �ɵ�?)
			ChargingAdditionalDamage = ((ElapsedChargeTime - ChargeTimeThreshold) / (MaxChargeTime - ChargeTimeThreshold)) * ChargingAdditionalDamageBase;
			AdditionalRecoilAmountPitch = ((ElapsedChargeTime - ChargeTimeThreshold) / (MaxChargeTime - ChargeTimeThreshold)) * ChargingAdditionalRecoilAmountPitchBase;
			AdditionalRecoilAmountYaw = ((ElapsedChargeTime - ChargeTimeThreshold) / (MaxChargeTime - ChargeTimeThreshold)) * ChargingAdditionalRecoilAmountYawBase;
			AdditionalProjectileRadius = ((ElapsedChargeTime - ChargeTimeThreshold) / (MaxChargeTime - ChargeTimeThreshold)) * ChargingAdditionalProjectileRadiusBase;
			PenetrableObjectsNum = ((ElapsedChargeTime - ChargeTimeThreshold) / (MaxChargeTime - ChargeTimeThreshold)) * MaxPenetrableObjectsNum_Left; //TODO: ������ ���� �޸� �ؾ���
			AdditionalPelletNum = ((ElapsedChargeTime - ChargeTimeThreshold) / (MaxChargeTime - ChargeTimeThreshold)) * ChargingAdditionalPelletMaxNum;
			UE_LOG(LogTemp, Error, TEXT("Penetrable Num: %d"), PenetrableObjectsNum);
			//UE_LOG(LogTemp, Error, TEXT("Pellet Num: %d"), AdditionalPelletNum);
		}

		if (AdditionalPelletNum == 0 || ChargingAdditionalPelletMaxNum == 0)
		{
			//TODO: Charge�� Ŀ����ȭ �ϴϱ� �� �ڵ嵵 �޸� ǥ���ؾ���
			StartSingleShot(true, true, PenetrableObjectsNum, ChargingAdditionalDamage, AdditionalRecoilAmountPitch, AdditionalRecoilAmountYaw, AdditionalProjectileRadius);
		}
		else
		{
			//TODO: �¿� �����ؾ���
			StartSingleShot(false, false, PenetrableObjectsNum, ChargingAdditionalDamage, AdditionalRecoilAmountPitch, AdditionalRecoilAmountYaw, AdditionalProjectileRadius, AdditionalPelletNum);
		}


		ElapsedChargeTime = 0.f;
	}
}
#pragma endregion

#pragma region Skill/Targeting
void AWeapon::HandleTargetDetectionSkill()
{
	if (CurrentState == IdleState && bCanUseTargetingSkill)
	{
		if (!TryTakeControl()) { return; };
		ChangeState(TargetingState);
		PlayWeaponSound(TargetSearchLoopSound);
		if (TargetingSkillWidget) { TargetingSkillWidget->SetDetectionTimeUIVisible(true); }
		UpdateTargetDetectionSkill(GetWorld()->GetDeltaSeconds());
		
		//suhyeon
		//UE_LOG(LogTemp, Warning, TEXT("RocketLauncherSkillActivated 델리게이트 호출됨"));
		OnRocketLauncherSkillActivated.Broadcast();
		
	}
	else if (CurrentState == TargetingState)
	{
		CancelTargetingSkill();
	}
}
void AWeapon::UpdateTargetDetectionSkill(float DeltaTime)
{
	ElapsedTimeAfterTargetingStarted += DeltaTime;
	TArray<AActor*> NewOverlappedActors;
	CurrentTargetDetectionRadius = (FMath::Clamp(ElapsedTimeAfterTargetingStarted, 0.f, TimeToReachMaxTargetDetectionRange) / TimeToReachMaxTargetDetectionRange) * MaxTargetDetectionRadius;
	CurrentTargetDetectionAngle = (FMath::Clamp(ElapsedTimeAfterTargetingStarted, 0.f, TimeToReachMaxTargetDetectionRange) / TimeToReachMaxTargetDetectionRange) * MaxTargetDetectionAngle;
	SearchOverlappedActor(Character->GetActorLocation(), CurrentTargetDetectionRadius, NewOverlappedActors);

	//TODO: Targets�� ���� Update�� �ʿ���. �׾����� Targets���� ���ܽ��Ѿ���
	// TargetMarker�� ��쿡�� Visibility�� false�� �ٲ��ִ� ������ �����ϱ�

	for (TSet<AActor*>::TIterator It = Targets.CreateIterator(); It; ++It)
	{
		AActor* PreviousTarget = *It;
		if (IsValid(PreviousTarget) && CheckIfTargetIsBlockedByObstacle(PreviousTarget))
		{
			UTargetMarkerWidget** TargetMarkerPtr = MapTargetActorToWidget.Find(PreviousTarget);
			(*TargetMarkerPtr)->RemoveFromViewport();
			(*TargetMarkerPtr)->RemoveFromParent();
			TargetMarkerWidgets.Remove(*TargetMarkerPtr);
			
			It.RemoveCurrent();
			MapTargetActorToWidget.Remove(PreviousTarget);
		}
	}

	for (AActor* NewOverlappedActor : NewOverlappedActors)
	{
		if (Targets.Num() >= MaxTargetNum || ElapsedTimeAfterTargetingStarted > MaxTargetDetectionTime)
		{
			break;
		}
		if (!Targets.Contains(NewOverlappedActor))
		{
			if (IsInViewport(GetScreenPositionOfWorldLocation(NewOverlappedActor->GetActorLocation()).Get<0>(), 1.f, 1.f))
			{
				if (GetUnsignedAngleBetweenVectors(Character->GetActorForwardVector(), NewOverlappedActor->GetActorLocation() - Character->GetActorLocation(), FVector::ZAxisVector) < CurrentTargetDetectionAngle)
				{
					if (!CheckIfTargetIsBlockedByObstacle(NewOverlappedActor))
					{
						Targets.Add(NewOverlappedActor);
						UTargetMarkerWidget* NewTargetMarker = Cast<UTargetMarkerWidget>(CreateTargetMarkerWidget(NewOverlappedActor));
						if (NewTargetMarker)
						{
							float TargetingReadyDuration = 0.3f; // default
							float TargetingSuccessDuration = 0.3f; // default
							if (Character)
							{
								UACUIMangerComponent* UIManager = Character->GetUIManager();
								if (UIManager)
								{
									UDataTable* UIDataTable = UIManager->GetUIDataTable();
									if (UIDataTable)
									{
										if (const FUIData* Row = UIDataTable->FindRow<FUIData>(TEXT("TargetingReady"), TEXT("")))
										{
											TargetingReadyDuration = Row->AnimDuration;
										}
										
										if (const FUIData* Row = UIDataTable->FindRow<FUIData>(TEXT("TargetingSuccess"), TEXT("")))
										{
											TargetingSuccessDuration = Row->AnimDuration;
										}
									}
								}
							}
							NewTargetMarker->StartLockOnProcess(TargetingReadyDuration, TargetingSuccessDuration);
							
							MapTargetActorToWidget.Add(NewOverlappedActor, NewTargetMarker); //suhyeon
							TargetMarkerWidgets.Add(NewTargetMarker);
							NewTargetMarker->AddToViewport();
							NewTargetMarker->SetAlignmentInViewport(FVector2D(0.5f, 0.5f));
							NewTargetMarker->SetVisibility(ESlateVisibility::Hidden);
						}

						if (TargetLockedSound)
						{
							UGameplayStatics::PlaySoundAtLocation(this, TargetLockedSound, Character->GetActorLocation());
						}
					}
				}
			}
		}
	}

	if (TargetingSkillWidget) 
	{ 
		TargetingSkillWidget->SetTargetingTimeUI(FMath::Clamp(ElapsedTimeAfterTargetingStarted, 0.f, MaxTargetingTime));
	}

	if (ElapsedTimeAfterTargetingStarted >= MaxTargetingTime)
	{
		CancelTargetingSkill();
	}
	else
	{
		float DeltaSeconds = GetWorld()->GetDeltaSeconds();
		GetWorld()->GetTimerManager().SetTimer(TargetDetectionTimer, [this, DeltaSeconds]() {UpdateTargetDetectionSkill(DeltaSeconds); }, DeltaSeconds, false);
	}
}
void AWeapon::HandleTargetingSkillFire(bool bIsLeftInput, bool bSingleProjectile, int32 NumPenetrable)
{
	if (CurrentState == TargetingState)
	{
		GetWorld()->GetTimerManager().ClearTimer(TargetDetectionTimer);

		ElapsedTimeAfterTargetingStarted = 0.f;
		CurrentTargetDetectionRadius = 0.f;
		CurrentTargetDetectionAngle = 0.f;

		//ResetTargetMarkers();
		StopWeaponSound();

		if (TargetingSkillWidget) { TargetingSkillWidget->SetDetectionTimeUIVisible(false); }

		//TArray<AActor*> TargetsArray = Targets.Array();

		// 락온이 완료된 타겟만 저장할 새로운 배열 선언
		TArray<AActor*> LockedOnTargets;
		
		// 모든 타겟을 순회하면 락온된 타겟만 선별
		for (AActor* Target : Targets)
		{
			UTargetMarkerWidget** TargetMarkerPtr = MapTargetActorToWidget.Find(Target);
			if (TargetMarkerPtr && *TargetMarkerPtr)
			{
				UTargetMarkerWidget* TargetMarker = Cast<UTargetMarkerWidget>(*TargetMarkerPtr);

				if (TargetMarker && TargetMarker->bIsLockedOn)
				{
					LockedOnTargets.Add(Target);
				}
			}
		}
		ResetTargetMarkers();
		Targets.Empty();

		bCanUseTargetingSkill = false;
		bool bflag = true;
		GetWorld()->GetTimerManager().SetTimer(TargetingSkillTimer, [this, bflag]() {EnableTargetingSkill(bflag); }, TargetingSkillCoolDown, false);
		
		if (LockedOnTargets.Num() == 0)
		{
			StartMissileLaunch(LockedOnTargets, &FireData_Skill);
			if (CurrentState == IdleState)
			{
				ChangeState(FiringState);
				StartSingleShot(bIsLeftInput, bSingleProjectile, NumPenetrable);
			}
		}
		else
		{
			StartMissileLaunch(LockedOnTargets, &FireData_Skill);
		}
	}
	else if (CurrentState == IdleState)
	{
		if (!bAllowNormalFireForSkillWeapon) { return; }
		ChangeState(FiringState);
		StartSingleShot(bIsLeftInput, bSingleProjectile, NumPenetrable);
	}
	else if (CurrentState == PumpActionReloadingState)
	{
		if (!bAllowNormalFireForSkillWeapon) { return; }
		BufferedFireRequest = FBufferedFireRequest(EWeaponAction::WeaponAction_SingleShot, bIsLeftInput, bSingleProjectile, NumPenetrable);
	}
	//suhyeon
	// 스킬 취소 시 델리게이트를 브로드캐스트하여 위젯에 알림
	OnRocketLauncherSkillOvered.Broadcast();
}
void AWeapon::CancelTargetingSkill()
{
	GetWorld()->GetTimerManager().ClearTimer(TargetDetectionTimer);

	ElapsedTimeAfterTargetingStarted = 0.f;
	CurrentTargetDetectionRadius = 0.f;
	CurrentTargetDetectionAngle = 0.f;

	ResetTargetMarkers();
	StopWeaponSound();

	Targets.Empty();

	ConfirmedTargets.Empty();
	CurrentTargetIndex = 0;

	if (TargetingSkillWidget) { TargetingSkillWidget->SetDetectionTimeUIVisible(false); }

	ReleaseControl();
	ChangeState(IdleState);

	//suhyeon
	// 스킬 취소 시 델리게이트를 브로드캐스트하여 위젯에 알림
	OnRocketLauncherSkillOvered.Broadcast();
}
void AWeapon::EnableTargetingSkill(bool bflag)
{
	bCanUseTargetingSkill = bflag;

	if (TargetingSkillWidget)
	{
		TargetingSkillWidget->SetTargetingSkillCoolDown(TargetingSkillCoolDown);
	}
}
void AWeapon::UpdateTargetingSkillUI()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(TargetingSkillTimer))
	{
		float ElapsedTime = GetWorld()->GetTimerManager().GetTimerElapsed(TargetingSkillTimer);
		ElapsedTime = FMath::Clamp(ElapsedTime, 0.f, TargetingSkillCoolDown);

		if (TargetingSkillWidget)
		{
			TargetingSkillWidget->SetTargetingSkillCoolDown(ElapsedTime);
		}
	}
}
bool AWeapon::TryTakeControl()
{
	if (!Character) { return false; }
	UWeaponSystemComponent* WSC = Character->GetWeaponSystemComponent();
	if (!WSC) { return false; }
	return WSC->TryTakeControl(this);
}
void AWeapon::ReleaseControl()
{
	if (!Character) { return; }
	UWeaponSystemComponent* WSC = Character->GetWeaponSystemComponent();
	if (!WSC) { return; }
	WSC->ReleaseControl();
}
void AWeapon::SetGlobalTimeDilation(float targettimescale)
{
	TargetGlobalTimeScale = targettimescale;
	bIsGlobalTimeScaleChanging = true;
}
void AWeapon::UpdateGlobalTimeDiation(float DeltaTime)
{
	if (bIsGlobalTimeScaleChanging)
	{
		float Current = GetWorld()->GetWorldSettings()->GetEffectiveTimeDilation();
		float New;
		if (TargetGlobalTimeScale <= Current)
		{
			New = FMath::FInterpTo(Current, TargetGlobalTimeScale, DeltaTime, TargetingGlobalTimeDilationSpeed_In);
		}
		else
		{
			New = FMath::FInterpTo(Current, TargetGlobalTimeScale, DeltaTime, TargetingGlobalTimeDilationSpeed_Out);
		}

		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), New);

		if (FMath::IsNearlyEqual(New, TargetGlobalTimeScale, 0.1f))
		{
			UE_LOG(LogTemp, Warning, TEXT("Global Time Scale is Stabilized!!!"));
			bIsGlobalTimeScaleChanging = false;
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), TargetGlobalTimeScale);
		}
	}
}
#pragma endregion

#pragma region Recoil/Aim
void AWeapon::AddRecoilValue(FWeaponRecoilStruct* RecoilStruct, float AdditionalRecoilAmountPitch, float AdditionalRecoilAmountYaw)
{
	bIsRecoiling = true;

	if (RecoilStruct)
	{
		CurrentRecoil = RecoilStruct;

		float RandRecoilPitch = FMath::RandRange((RecoilStruct->RecoilAmountPitch + AdditionalRecoilAmountPitch) * RecoilStruct->RecoilRangeMinPitch, (RecoilStruct->RecoilAmountPitch + AdditionalRecoilAmountPitch) * RecoilStruct->RecoilRangeMaxPitch) * (-1);
		float RandRecoilYaw = FMath::RandRange((RecoilStruct->RecoilAmountYaw + AdditionalRecoilAmountYaw) * RecoilStruct->RecoilRangeMinYaw, (RecoilStruct->RecoilAmountYaw + AdditionalRecoilAmountYaw) * RecoilStruct->RecoilRangeMaxYaw);

		TotalTargetRecoilValuePitch += RandRecoilPitch;
		TotalTargetRecoilValueYaw += RandRecoilYaw;
	}
}

void AWeapon::ApplyRecoil(float DeltaTime, FWeaponRecoilStruct* RecoilStruct)
{
	if (Character && RecoilStruct)
	{
		APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
		if (!PlayerController) return;

		float InterpRecoilTargetValue_Pitch = FMath::FInterpTo(0.f, TotalTargetRecoilValuePitch - CulmulatedRecoilValuePitch, DeltaTime, RecoilStruct->RecoilSpeed);
		float InterpRecoilTargetValue_Yaw = FMath::FInterpTo(0.f, TotalTargetRecoilValueYaw - CulmulatedRecoilValueYaw, DeltaTime, RecoilStruct->RecoilSpeed);;

		PlayerController->AddPitchInput(InterpRecoilTargetValue_Pitch);
		PlayerController->AddYawInput(InterpRecoilTargetValue_Yaw);

		CulmulatedRecoilValuePitch += InterpRecoilTargetValue_Pitch;
		CulmulatedRecoilValueYaw += InterpRecoilTargetValue_Yaw;
	}
}

void AWeapon::RecoverRecoil(float DeltaTime, FWeaponRecoilStruct* RecoilStruct)
{
	APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
	if (!PlayerController) return;

	if (!RecoilStruct) return;

	if (RecoilStruct->bIsRecoilRecoverAffectedByPlayerInput)
	{
		FVector2D PlayerLookInputVector2D = Character->GetPlayerLookInputVector();

		if (FMath::Sign(PlayerLookInputVector2D.Y) == FMath::Sign((CulmulatedRecoilValuePitch - RecoveredRecoilValuePitch) * (-1)))
		{
			if (PlayerLookInputVector2D.Y >= 0)
			{
				RecoveredRecoilValuePitch += (-1) * FMath::Clamp(PlayerLookInputVector2D.Y, 0.f, (CulmulatedRecoilValuePitch - RecoveredRecoilValuePitch) * (-1));
			}
			else
			{
				RecoveredRecoilValuePitch += (-1) * FMath::Clamp(PlayerLookInputVector2D.Y, (CulmulatedRecoilValuePitch - RecoveredRecoilValuePitch) * (-1), 0.f);
			}
		}

		if (FMath::Sign(PlayerLookInputVector2D.X) == FMath::Sign((CulmulatedRecoilValueYaw - RecoveredRecoilValueYaw) * (-1)))
		{
			if (PlayerLookInputVector2D.X >= 0)
			{
				RecoveredRecoilValueYaw += (-1) * FMath::Clamp(PlayerLookInputVector2D.X, 0.f, (CulmulatedRecoilValueYaw - RecoveredRecoilValueYaw) * (-1));
			}
			else
			{
				RecoveredRecoilValueYaw += (-1) * FMath::Clamp(PlayerLookInputVector2D.X, (CulmulatedRecoilValueYaw - RecoveredRecoilValueYaw) * (-1), 0.f);
			}
		}

		float InterpRecoilRecoverTargetValue_Pitch = FMath::FInterpTo(0.f, CulmulatedRecoilValuePitch - RecoveredRecoilValuePitch, DeltaTime, RecoilStruct->RecoilRecoverSpeed);
		float InterpRecoilRecoverTargetValue_Yaw = FMath::FInterpTo(0.f, CulmulatedRecoilValueYaw - RecoveredRecoilValueYaw, DeltaTime, RecoilStruct->RecoilRecoverSpeed);;

		PlayerController->AddPitchInput(-InterpRecoilRecoverTargetValue_Pitch);
		PlayerController->AddYawInput(-InterpRecoilRecoverTargetValue_Yaw);

		RecoveredRecoilValuePitch += InterpRecoilRecoverTargetValue_Pitch;
		RecoveredRecoilValueYaw += InterpRecoilRecoverTargetValue_Yaw;
	}
	else
	{
		float InterpRecoilRecoverTargetValue_Pitch = FMath::FInterpTo(0.f, CulmulatedRecoilValuePitch - RecoveredRecoilValuePitch, DeltaTime, RecoilStruct->RecoilRecoverSpeed);
		float InterpRecoilRecoverTargetValue_Yaw = FMath::FInterpTo(0.f, CulmulatedRecoilValueYaw - RecoveredRecoilValueYaw, DeltaTime, RecoilStruct->RecoilRecoverSpeed);

		PlayerController->AddPitchInput(-InterpRecoilRecoverTargetValue_Pitch);
		PlayerController->AddYawInput(-InterpRecoilRecoverTargetValue_Yaw);

		RecoveredRecoilValuePitch += InterpRecoilRecoverTargetValue_Pitch;
		RecoveredRecoilValueYaw += InterpRecoilRecoverTargetValue_Yaw;
	}

	if (FMath::Abs(CulmulatedRecoilValuePitch - RecoveredRecoilValuePitch) < KINDA_SMALL_NUMBER
		&& FMath::Abs(CulmulatedRecoilValueYaw - RecoveredRecoilValueYaw) < KINDA_SMALL_NUMBER)
	{
		UE_LOG(LogTemp, Warning, TEXT("Recoil has been perfectly Recovered!!!"));

		TotalTargetRecoilValuePitch = 0.f;
		TotalTargetRecoilValueYaw = 0.f;

		CulmulatedRecoilValuePitch = 0.f;
		CulmulatedRecoilValueYaw = 0.f;

		RecoveredRecoilValuePitch = 0.f;
		RecoveredRecoilValueYaw = 0.f;

		bIsRecoiling = false;
	}
}
void AWeapon::UpdateRecoil(float DeltaTime)
{
	if (Character)
	{
		if (bIsRecoiling)
		{
			if (bIsZoomIn)
			{
				ApplyRecoil(DeltaTime, &ZoomRecoil);
				RecoverRecoil(DeltaTime, &ZoomRecoil);
			}
			else
			{
				ApplyRecoil(DeltaTime, CurrentRecoil);
				RecoverRecoil(DeltaTime, CurrentRecoil);
			}
		}
	}
}
#pragma endregion

#pragma region Recoil/ArmAnimation
void AWeapon::AddArmRecoil(FArmRecoilStruct* armrecoil)
{
	if (armrecoil != nullptr && CharacterAnimInstance->GetClass()->ImplementsInterface(UWeaponInterface::StaticClass()))
	{
		Cast<IWeaponInterface>(CharacterAnimInstance)->AddArmRecoil(armrecoil);
	}
}
FArmRecoilStruct* AWeapon::GetArmRecoilInfo()
{
	return &ArmRecoil_L;
}
FArmRecoilStruct* AWeapon::GetArmRecoilInfo_Hand()
{
	return &ArmRecoil_Hand;
}
FArmRecoilStruct* AWeapon::GetArmRecoilInfo_UpperArm()
{
	return &ArmRecoil_UpperArm;
}
FArmRecoilStruct* AWeapon::GetArmRecoilInfo_LowerArm()
{
	return &ArmRecoil_LowerArm;
}
#pragma endregion

#pragma region Recoil/SkilWeapon
void AWeapon::AddSkillWeaponRecoil(FArmRecoilStruct* armrecoil)
{
	if (armrecoil != nullptr && CharacterAnimInstance->GetClass()->ImplementsInterface(UWeaponInterface::StaticClass()))
	{
		Cast<IWeaponInterface>(CharacterAnimInstance)->AddSkillWeaponRecoil(armrecoil);
	}
}
#pragma endregion

#pragma region Overheat
void AWeapon::AddOverheatValue()
{
	bIsOverheating = true;
	OverheatRecoverTimer = 0.f;

	TotalTargetOverheatValue += OverheatBaseIncrement; // TODO: �ѱ� �ɷ� ��� �ð� ��ŭ ����������
	TotalTargetOverheatValue = FMath::Clamp(TotalTargetOverheatValue, 0.f, MaxOverheatValue);
}
void AWeapon::ApplyOverheat(float DeltaTime)
{
	float InterpOverheatTargetValue = FMath::FInterpConstantTo(CurrentOverheatVaule, TotalTargetOverheatValue, DeltaTime, OverheatSpeed);
	CurrentOverheatVaule = InterpOverheatTargetValue;

	OverheatRecoverTimer += DeltaTime;
}
void AWeapon::RecoverOverheat(float DeltaTime)
{
	float InterpOverheatRecoverTargetValue = FMath::FInterpConstantTo(CurrentOverheatVaule, 0.f, DeltaTime, OverheatRecoverSpeed);
	CurrentOverheatVaule = InterpOverheatRecoverTargetValue;

	if (FMath::Abs(CurrentOverheatVaule) < 0.1f)
	{
		UE_LOG(LogTemp, Error, TEXT("Overheat has been perfectly Recovered!!!"));

		TotalTargetOverheatValue = 0.f;
		CurrentOverheatVaule = 0.f;

		bIsOverheating = false;

		//TODO: Overheat UI ������Ʈ �ؾ���
	}
}
void AWeapon::UpdateOverheat(float DeltaTime)
{
	if (Character)
	{
		if (bIsOverheating)
		{
			if (DefaultSpread.SpreadRecoveryStartTime > SpreadRecoverTimer)
			{
				ApplyOverheat(DeltaTime);
			}
			else
			{
				RecoverOverheat(DeltaTime);
			}

			//TODO: Overheat UI Update
		}
	}
}
#pragma endregion

#pragma region Projectile/SingleProjectileSpread
void AWeapon::AddSpreadValue(FProjectileSpreadValue* SpreadValue)
{
	if (!SpreadValue) return;

	bIsSpreading = true;
	SpreadRecoverTimer = 0.f;

	float RandSpreadValue = FMath::RandRange((SpreadValue->SpreadAmountBase) * SpreadValue->SpreadRangeMin, (SpreadValue->SpreadAmountBase) * SpreadValue->SpreadRangeMax);

	TotalTargetSpreadValue += RandSpreadValue;
	TotalTargetSpreadValue = FMath::Clamp(TotalTargetSpreadValue, 0.f, SpreadValue->MaxSpreadValue);
}
void AWeapon::ApplySpread(float DeltaTime, FProjectileSpreadValue* SpreadValue)
{
	if (!SpreadValue) return;

	float InterpSpreadTargetValue = FMath::FInterpConstantTo(CurrentSpreadVaule, TotalTargetSpreadValue, DeltaTime, SpreadValue->SpreadSpeed);
	CurrentSpreadVaule = InterpSpreadTargetValue;

	SpreadRecoverTimer += DeltaTime;
}
void AWeapon::RecoverSpread(float DeltaTime, FProjectileSpreadValue* SpreadValue)
{
	if (!SpreadValue) return;

	float InterpSpreadRecoverTargetValue = FMath::FInterpConstantTo(CurrentSpreadVaule, 0.f, DeltaTime, SpreadValue->SpreadRecoverSpeed);
	CurrentSpreadVaule = InterpSpreadRecoverTargetValue;

	if (FMath::Abs(CurrentSpreadVaule) < 0.1f)
	{
		UE_LOG(LogTemp, Error, TEXT("Spread has been perfectly Recovered!!!"));

		TotalTargetSpreadValue = 0.f;
		CurrentSpreadVaule = 0.f;

		bIsSpreading = false;

		if (AimUIWidget)
		{
			AimUIWidget->ResetAimUISize();
		}
	}
}
void AWeapon::UpdateSpread(float DeltaTime)
{
	if (Character)
	{
		if (bIsSpreading)
		{
			if (bIsZoomIn)
			{
				if (ZoomSpread.SpreadRecoveryStartTime > SpreadRecoverTimer)
				{
					ApplySpread(DeltaTime, &ZoomSpread);
				}
				else
				{
					RecoverSpread(DeltaTime, &ZoomSpread);
				}
			}
			else
			{
				if (DefaultSpread.SpreadRecoveryStartTime > SpreadRecoverTimer)
				{
					ApplySpread(DeltaTime, &DefaultSpread);
				}
				else
				{
					RecoverSpread(DeltaTime, &DefaultSpread);
				}

				if (DefaultSpread.bEnableProjectileSpread || DefaultSpread.bEnableAimUISpread)
				{
					if (AimUIWidget)
					{
						AimUIWidget->ApplyAimUISpread(CurrentSpreadVaule);
					}
				}
			}
		}
	}
}
FVector AWeapon::GetRandomSpreadVector(FVector BaseDir)
{
	const FVector SpreadVector = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(BaseDir.GetSafeNormal(), CurrentSpreadVaule);

	return SpreadVector;
}
#pragma endregion

#pragma region Camera
void AWeapon::StartCameraSettingChange(FWeaponCamSettingValue* CamSetting)
{
	bIsUsingPlayerCamFov = true;

	if (GetWorld()->GetTimerManager().IsTimerActive(CamSettingTimer))
	{
		GetWorld()->GetTimerManager().ClearTimer(CamSettingTimer);
	}

	//UpdateCameraSetting(GetWorld()->GetDeltaSeconds(), CamSetting);
}
void AWeapon::UpdateCameraSetting(float DeltaTime, FWeaponCamSettingValue* CamSetting)
{
	if (Character)
	{
		UCameraComponent* Camera = Character->GetCameraComponent();
		if (Camera)
		{
			FRotator NewCamRotation = FMath::RInterpTo(Camera->GetRelativeRotation(), CamSetting->CameraRelativeRotation, DeltaTime, CamSetting->CameraRelativeRotation_InterpSpeed_ToThisState);
			FVector NewCamLocation = FMath::VInterpTo(Camera->GetRelativeLocation(), CamSetting->CameraRelativeLocation, DeltaTime, CamSetting->CameraRelativeLocation_InterpSpeed_ToThisState);
			float NewCamFOV = FMath::FInterpTo(Camera->FieldOfView, CamSetting->FOV, DeltaTime, CamSetting->FOV_InterpSpeed_ToThisState);

			Camera->SetRelativeRotation(NewCamRotation);
			Camera->SetRelativeLocation(NewCamLocation);
			Camera->SetFieldOfView(NewCamFOV);

			FRotator CameraRelativeRotation_Error = CamSetting->CameraRelativeRotation - Camera->GetRelativeRotation();
			CameraRelativeRotation_Error.Normalize();

			//if (CameraRelativeRotation_Error.IsNearlyZero()
			//	&& FVector::Dist(CamSetting->CameraRelativeLocation, Camera->GetRelativeLocation()) < 0.01
			//	//&& FMath::Abs(CamSetting->FOV - Camera->FieldOfView) < KINDA_SMALL_NUMBER
			//	&& FMath::Abs(CamSetting->FOV - Camera->FieldOfView) < 0.05)
			//{
			//	StopCameraSettingChange();
			//}
			if (FVector::Dist(CamSetting->CameraRelativeLocation, Camera->GetRelativeLocation()) < 0.01
				&& FMath::Abs(CamSetting->FOV - Camera->FieldOfView) < 0.01)
			{
				StopCameraSettingChange();
			}
			else
			{
				float DeltaSecond = GetWorld()->GetDeltaSeconds();
				GetWorld()->GetTimerManager().SetTimer(CamSettingTimer, [this, DeltaSecond, CamSetting]() {UpdateCameraSetting(DeltaSecond, CamSetting); }, DeltaSecond, false);
			}
		}
	}
}
void AWeapon::StopCameraSettingChange()
{
	bIsUsingPlayerCamFov = false;
	UE_LOG(LogTemp, Error, TEXT("Modifying Cam Setting is Completed!!!"));
}
void AWeapon::ForceStopCamModification()
{
	bIsUsingPlayerCamFov = false;

	if (GetWorld()->GetTimerManager().IsTimerActive(CamSettingTimer))
	{
		GetWorld()->GetTimerManager().ClearTimer(CamSettingTimer);
	}
}
void AWeapon::ApplyCameraShake(TSubclassOf<UWeaponCameraShakeBase> CamShakeClass, float Scale)
{
	if (Character && CamShakeClass)
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
		{
			PlayerController->PlayerCameraManager->StartCameraShake(CamShakeClass, Scale);
		}
	}
}
#pragma endregion