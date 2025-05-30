
#include "ActorComponents/WeaponSystem/ACWeapon.h"
//#include "Characters/Player/SuraCharacterPlayer.h"
#include "ActorComponents/WeaponSystem/SuraCharacterPlayerWeapon.h"

#include "ActorComponents/WeaponSystem/SuraProjectile.h"
#include "ActorComponents/WeaponSystem/WeaponInterface.h"
#include "ActorComponents/WeaponSystem/WeaponSystemComponent.h"

#include "ActorComponents/WeaponSystem/SuraWeaponBaseState.h"
#include "ActorComponents/WeaponSystem/SuraWeaponIdleState.h"
#include "ActorComponents/WeaponSystem/SuraWeaponFiringState.h"
#include "ActorComponents/WeaponSystem/SuraWeaponUnequippedState.h"
#include "ActorComponents/WeaponSystem/SuraWeaponReloadingState.h"
#include "ActorComponents/WeaponSystem/SuraWeaponSwitchingState.h"
#include "ActorComponents/WeaponSystem/SuraWeaponTargetingState.h"
#include "ActorComponents/WeaponSystem/SuraWeaponChargingState.h"
#include "ActorComponents/WeaponSystem/WeaponCameraShakeBase.h"
#include "ActorComponents/WeaponSystem/AmmoCounterWidget.h"
#include "ActorComponents/WeaponSystem/WeaponAimUIWidget.h"


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

#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

//----------------------------------
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/CanvasRenderTarget2D.h"



// Sets default values for this component's properties
AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	SetRootComponent(WeaponMesh); // suhyeon
	WeaponMesh->SetupAttachment(GetRootComponent(), FName(TEXT("WeaponMesh")));
	WeaponMesh->SetVisibility(false);
	
	WeaponAnimInstance = WeaponMesh->GetAnimInstance();

	//TODO: Muzzle Offset�� weapon Type�� ���� �ٸ���
	// ����� �⺻������ Weapon Mesh��"Muzzle" Socket�� ��ġ�� �̿��ϰ� �־ MuzzleOffset�� ��� ���ϴ� ����
	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);

	// Fire Mode
	//TODO: �ϴ��� �⺻ Single�� �ϴµ�, WeaponName�� ���� �����ڿ��� �������ִ� ������ �����ϱ�
	WeaponName = EWeaponName::WeaponName_Rifle;
	WeaponType = EWeaponType::WeaponType_Rifle;
	WeaponFireMode = EWeaponFireMode::WeaponFireMode_Single;
	
	WeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeaponMesh->SetCollisionObjectType(ECC_GameTraceChannel3); //Weapon
	WeaponMesh->SetCollisionResponseToAllChannels(ECR_Ignore);

	//---------------------------------------------------------------------------------
	LeftAmmoInCurrentMag = MaxAmmoPerMag;

	//<UI>
	//AmmoCounterWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("AmmoCounterWidgetComponent"));
	//AmmoCounterWidgetComponent->SetDrawSize(FVector2D());
	//AmmoCounterWidgetComponent->SetHiddenInGame(false);

	//  / Script / UMGEditor.WidgetBlueprint'/Game/FPWeapon/UI/WBP_AmmoCounter_2.WBP_AmmoCounter_2'
	// E: / Unreal_engine_projects / PJSURA_2 / SuraS / Content / FPWeapon / UI / WBP_AmmoCounter_2.uasset


	if (AmmoCounterWidgetComponent)
	{
		//UE_LOG(LogTemp, Error, TEXT("AmmoCounterWidgetComponent Is Available!!!"));
		//AmmoCounterWidgetComponent->SetupAttachment(this, FName(TEXT("AmmoCounter")));

		//AmmoCounterWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
		//AmmoCounterWidgetComponent->SetVisibility(true);
		//AmmoCounterWidgetComponent->SetHiddenInGame(false);
		//AmmoCounterWidgetComponent->SetRelativeLocation(FVector(0.f, 0.f, 0.f));

		//static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass{ TEXT("/Game/UI/Enemies/WBP_EnemyHealthBar") };

		//if (WidgetClass.Succeeded())
		//{
		//	AmmoCounterWidgetComponent->SetWidgetClass((WidgetClass.Class));
		//}

		//static ConstructorHelpers::FClassFinder<UAmmoCounterWidget> WidgetClass{ TEXT("/Game/FPWeapon/UI/WBP_AmmoCounter_2") };

		//if (WidgetClass.Succeeded())
		//{
		//	UE_LOG(LogTemp, Error, TEXT("AmmoCounterWidgetClass Is Available!!!"));
		//	//AmmoCounterWidgetComponent->SetWidgetClass(AmmoCounterWidgetClass);
		//	AmmoCounterWidgetComponent->SetWidgetClass(WidgetClass.Class);
		//	AmmoCounterWidget = Cast<UAmmoCounterWidget>(AmmoCounterWidgetComponent->GetWidget());
		//	if (AmmoCounterWidget)
		//	{
		//		UE_LOG(LogTemp, Error, TEXT("AmmoCounterWidget Is Available!!!"));

		//	}




		//	//AmmoCounterWidget = CreateWidget<UAmmoCounterWidget>(GetWorld(), AmmoCounterWidgetClass, FName(TEXT("AmmoCounterWidget")));
		//	//if (AmmoCounterWidget)
		//	//{
		//	//	UE_LOG(LogTemp, Error, TEXT("AmmoCounterWidget Is Available!!!"));
		//	//	AmmoCounterWidgetComponent->SetWidget(AmmoCounterWidget);
		//	//	if (AmmoCounterWidgetComponent->GetWidget() == AmmoCounterWidget)
		//	//	{
		//	//		UE_LOG(LogTemp, Error, TEXT("AmmoCounterWidget Is Set!!!"));
		//	//	}
		//	//}
		//}

	}



	//if (AmmoCounterWidgetClass)
	//{
	//	AmmoCounterWidget = CreateWidget<UAmmoCounterWidget>(GetWorld(), AmmoCounterWidgetClass);
	//	if (AmmoCounterWidgetComponent)
	//	{
	//		//AmmoCounterWidgetComponent->SetWidgetClass(AmmoCounterWidget->GetClass());
	//		//AmmoCounterWidget = AmmoCounterWidgetComponent->GetWidget();
	//		AmmoCounterWidgetComponent->SetWidget(AmmoCounterWidget);

	//		if (AmmoCounterWidget)
	//		{
	//			UE_LOG(LogTemp, Error, TEXT("AmmoCounterWidget is Availlable"));
	//		}

	//		//AmmoCounterWidget->AddToViewport();
	//		//AmmoCounterWidget->SetVisibility(ESlateVisibility::Visible);

	//		//AmmoCounterWidget->AddToRoot();
	//		//AmmoCounterWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	//		AmmoCounterWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
	//		//AmmoCounterWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	//		AmmoCounterWidgetComponent->SetDrawAtDesiredSize(true);
	//		AmmoCounterWidgetComponent->SetDrawSize(FVector2D(500.f, 500.f));

	//		AmmoCounterWidgetComponent->SetHiddenInGame(false);
	//		AmmoCounterWidgetComponent->SetVisibility(true);

	//		FAttachmentTransformRules AttachRule(EAttachmentRule::KeepRelative, true);
	//		AmmoCounterWidgetComponent->AttachToComponent(this, AttachRule, FName(TEXT("AmmoCounter")));


	//		UE_LOG(LogTemp, Error, TEXT("AmmoCounterWidget!!!"));
	//	}
	//}


}

void AWeapon::InitializeWeapon(ASuraCharacterPlayerWeapon* NewCharacter)
{
	Character = NewCharacter;
	if (Character)
	{
		CharacterAnimInstance = Character->GetArmMesh()->GetAnimInstance();
		InitializeCamera(Character);
		LoadWeaponData();
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

	// Set Aim Socket Relative Transform
	SetAimSocketRelativeTransform();
}

void AWeapon::InitializeCamera(ASuraCharacterPlayerWeapon* NewCharacter)
{
	if (NewCharacter)
	{
		CamSetting_Default.CameraRelativeLocation = NewCharacter->GetCamera()->GetRelativeLocation();
		CamSetting_Default.CameraRelativeRotation = NewCharacter->GetCamera()->GetRelativeRotation();
		CamSetting_Default.FOV = NewCharacter->GetCamera()->FieldOfView;
	}
}

void AWeapon::InitializeUI()
{
	//if (CrosshairWidgetClass)
	//{
		//CrosshairWidget = CreateWidget<UUserWidget>(GetWorld(), CrosshairWidgetClass);
	//}

	if (AimUIWidgetClass)
	{
		AimUIWidget = CreateWidget<UWeaponAimUIWidget>(GetWorld(), AimUIWidgetClass);
	}

	if (AmmoCounterWidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("AmmoCounterWidgetClass Is Available!!!"));
		//AmmoCounterWidget = CreateWidget<UAmmoCounterWidget>(GetWorld(), AmmoCounterWidgetClass, FName(TEXT("AmmoCounterWidget")));
		AmmoCounterWidget = CreateWidget<UAmmoCounterWidget>(GetWorld(), AmmoCounterWidgetClass);
		//AmmoCounterWidget = CreateWidget<UUserWidget>(GetWorld(), AmmoCounterWidgetClass, FName(TEXT("AmmoCounterWidget")));
		//if (AmmoCounterWidget)
		//{
		//	//UE_LOG(LogTemp, Error, TEXT("AmmoCounterWidget Is Available!!!"));
		//	//AmmoCounterWidgetComponent->SetWidget(AmmoCounterWidget);
		//	if (AmmoCounterWidgetComponent->GetWidget() == AmmoCounterWidget)
		//	{
		//		UE_LOG(LogTemp, Error, TEXT("AmmoCounterWidget Is Set!!!"));

		//		FAttachmentTransformRules AttachRule(EAttachmentRule::SnapToTarget, true);
		//		AmmoCounterWidgetComponent->AttachToComponent(this, AttachRule, FName(TEXT("AmmoCounter")));


		//		//AmmoCounterWidget->AddToViewport();
		//		AmmoCounterWidget->SetVisibility(ESlateVisibility::Visible);

		//		AmmoCounterWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
		//		AmmoCounterWidgetComponent->SetDrawAtDesiredSize(true);
		//		AmmoCounterWidgetComponent->SetDrawSize(FVector2D(100.f, 100.f));

		//		AmmoCounterWidget->UpdateAmmoCount(NumOfLeftAmmo);
		//	}
		//}

		if (AmmoCounterWidget)
		{
			AmmoCounterWidget->UpdateAmmoCount(LeftAmmoInCurrentMag);
			AmmoCounterWidget->UpdateTotalAmmo(TotalAmmo);
		}
	}

	//----------------

	//if (AmmoCounterWidgetClass)
	//{
	//	AmmoCounterWidget = CreateWidget<UAmmoCounterWidget>(GetWorld(), AmmoCounterWidgetClass);
	//	if (AmmoCounterWidgetComponent)
	//	{
	//		//AmmoCounterWidgetComponent->SetWidgetClass(AmmoCounterWidget->GetClass());
	//		//AmmoCounterWidget = AmmoCounterWidgetComponent->GetWidget();
	//		AmmoCounterWidgetComponent->SetWidget(AmmoCounterWidget);

	//		if (AmmoCounterWidget)
	//		{
	//			UE_LOG(LogTemp, Error, TEXT("AmmoCounterWidget is Availlable"));
	//		}

	//		//AmmoCounterWidget->AddToViewport();
	//		//AmmoCounterWidget->SetVisibility(ESlateVisibility::Visible);

	//		//AmmoCounterWidget->AddToRoot();
	//		//AmmoCounterWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	//		AmmoCounterWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
	//		//AmmoCounterWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	//		AmmoCounterWidgetComponent->SetDrawAtDesiredSize(true);
	//		AmmoCounterWidgetComponent->SetDrawSize(FVector2D(500.f, 500.f));

	//		AmmoCounterWidgetComponent->SetHiddenInGame(false);
	//		AmmoCounterWidgetComponent->SetVisibility(true);
	//	
	//		FAttachmentTransformRules AttachRule(EAttachmentRule::KeepRelative, true);
	//		AmmoCounterWidgetComponent->AttachToComponent(this, AttachRule, FName(TEXT("AmmoCounter")));

	//		UE_LOG(LogTemp, Error, TEXT("AmmoCounterWidget!!!"));
	//	}
	//}
}

void AWeapon::LoadWeaponData()
{
	WeaponData = WeaponDataTableHandle.GetRow<FWeaponData>("");
	if (WeaponData)
	{
		// <Action>
		LeftMouseAction = WeaponData->LeftMouseAction;
		RightMouseAction = WeaponData->RightMouseAction;

		// <Projectile>
		LeftProjectileClass = WeaponData->LeftProjectileClass;
		RightProjectileClass = WeaponData->RightProjectileClass;

		// <Sound>
		FireSound = WeaponData->FireSound;
		ChargeSound = WeaponData->ChargeSound;

		// <Effect>
		MuzzleFireEffect = WeaponData->FireEffect;
		ChargeEffect = WeaponData->ChargeEffect;
		ChargeEffectLocation = WeaponData->ChargeEffectLocation;
		ChargeEffectRotation = WeaponData->ChargeEffectRotation;
		ChargeEffenctScale = WeaponData->ChargeEffenctScale;

		// <Reload>
		MaxTotalAmmo = WeaponData->MaxTotalAmmo;
		TotalAmmo = MaxTotalAmmo;
		MaxAmmoPerMag = WeaponData->MaxAmmoPerMag;
		LeftAmmoInCurrentMag = MaxAmmoPerMag;

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
		bEnableMultiProjectile_Left = WeaponData->bEnableMultiProjectile_Left;
		bEnableMultiProjectile_Right = WeaponData->bEnableMultiProjectile_Right;
		MaxAngleOfMultiProjectileSpread = WeaponData->MaxAngleOfMultiProjectileSpread;

		// <Recoil>
		DefaultRecoil = WeaponData->DefaultRecoil;
		ZoomRecoil = WeaponData->ZoomRecoil;

		// <Camera Shake>
		DefaultCameraShakeClass = WeaponData->DefaultCameraShakeClass;
		ZoomCameraShakeClass = WeaponData->ZoomCameraShakeClass;
		ChargingCameraShakeClass = WeaponData->ChargingCameraShakeClass;

		// <Targeting(Homing)>
		MissileLaunchDelay = WeaponData->MissileLaunchDelay;
		MaxTargetNum = WeaponData->MaxTargetNum;
		MaxTargetDetectionRadius = WeaponData->MaxTargetDetectionRadius;
		MaxTargetDetectionAngle = WeaponData->MaxTargetDetectionAngle;
		MaxTargetDetectionTime = WeaponData->MaxTargetDetectionTime;
		TimeToReachMaxTargetDetectionRange = WeaponData->TimeToReachMaxTargetDetectionRange;

		// <Charging>
		bAutoFireAtMaxChargeTime = WeaponData->bAutoFireAtMaxChargeTime;
		ChargeTimeThreshold = WeaponData->ChargeTimeThreshold;
		MaxChargeTime = WeaponData->MaxChargeTime;
		ChargingAdditionalDamageBase = WeaponData->ChargingAdditionalDamageBase;
		ChargingAdditionalRecoilAmountPitchBase = WeaponData->ChargingAdditionalRecoilAmountPitchBase;
		ChargingAdditionalRecoilAmountYawBase = WeaponData->ChargingAdditionalRecoilAmountYawBase;
		ChargingAdditionalProjectileRadiusBase = WeaponData->ChargingAdditionalProjectileRadiusBase;

		// <Penetration>
		MaxPenetrableObjectsNum_Left = WeaponData->MaxPenetrableObjectsNum_Left;
		MaxPenetrableObjectsNum_Right = WeaponData->MaxPenetrableObjectsNum_Right;
	}
}

// Called when the game starts
void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	//TODO: WeaponType�� ���� �����Ǵ� State ��ü���� �޶������� �����ϱ�
	// �������, ȣ�� ���� ���� ��쿡��, ChargingState Ȥ�� TargetingState ���� �߰��� �� ����
	IdleState = NewObject<USuraWeaponIdleState>(this, USuraWeaponIdleState::StaticClass());
	FiringState = NewObject<USuraWeaponFiringState>(this, USuraWeaponFiringState::StaticClass());
	UnequippedState = NewObject<USuraWeaponUnequippedState>(this, USuraWeaponUnequippedState::StaticClass());
	ReloadingState = NewObject<USuraWeaponReloadingState>(this, USuraWeaponReloadingState::StaticClass());
	SwitchingState = NewObject<USuraWeaponSwitchingState>(this, USuraWeaponSwitchingState::StaticClass());

	TargetingState = NewObject<USuraWeaponTargetingState>(this, USuraWeaponTargetingState::StaticClass());
	ChargingState = NewObject<USuraWeaponChargingState>(this, USuraWeaponChargingState::StaticClass());

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

	UpdateOverheat(DeltaTime); //TODO: Overheat ����� �Ⱦ��Ŷ�� �ϴµ�, Ȥ�� mesh�� ���� ȿ�� ������ ���� �����ϱ� �ϴ��� ����
}

void AWeapon::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

bool AWeapon::AttachWeaponToPlayer(ASuraCharacterPlayerWeapon* TargetCharacter)
{
	Character = TargetCharacter;

	if (Character == nullptr)
	{
		return false;
	}

	// Attach the weapon to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);

	//TODO: BP���� �Է��ϵ��� �ؾ���
	if (WeaponName == EWeaponName::WeaponName_Rifle)
	{
		AttachToComponent(Character->GetArmMesh(), AttachmentRules, FName(TEXT("Gun_Rifle")));
	}
	else if (WeaponName == EWeaponName::WeaponName_ShotGun)
	{
		AttachToComponent(Character->GetArmMesh(), AttachmentRules, FName(TEXT("Gun")));
	}
	else if (WeaponName == EWeaponName::WeaponName_MissileLauncher)
	{
		AttachToComponent(Character->GetArmMesh(), AttachmentRules, FName(TEXT("Gun_MissileLauncher")));
	}
	else if (WeaponName == EWeaponName::WeaponName_RailGun)
	{
		AttachToComponent(Character->GetArmMesh(), AttachmentRules, FName(TEXT("Gun_RailGun")));
	}


	//TODO: �� ���� ����� ������ ������
	//RightHandToAimSocketOffset = this->GetSocketLocation(FName(TEXT("Aim"))) - Character->GetMesh()->GetSocketLocation(FName("Gun"));
	RightHandToAimSocketOffset = WeaponMesh->GetSocketLocation(FName(TEXT("Aim"))) - Character->GetArmMesh()->GetBoneLocation(FName(TEXT("hand_r")));
	if (Character->GetWeaponSystem() && Character->GetWeaponSystem()->GetClass()->ImplementsInterface(UWeaponInterface::StaticClass()))
	{
		Character->GetWeaponSystem()->SetRightHandToAimSocketOffset(RightHandToAimSocketOffset);
	}

	// Set AimSocketTransform
	SetAimSocketTransform();

	// Set Up Widget UI Class
	// TODO: WidgetInstance ������ Weapon Initialize������ �����ϰ�, Ű�� ���� ��ɸ� ActivateCrosshairWidget���� �ϱ�
	ActivateCrosshairWidget(true);
	ActivateAmmoCounterWidget(true);

	//TODO: BP���� �ΰ������� ������ Mesh�鵵 Visibility�� �����ؾ���. �ٵ� �����Ϳ��� WeaponMesh�� �θ� �������� �Ǿ������� ������ �͵��� �˾Ƽ� ó���Ǵ� ��?
	WeaponMesh->SetVisibility(true);

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
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		//DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		//ActivateCrosshairWidget(false);
		WeaponMesh->SetVisibility(false);
	}
}

void AWeapon::FireSingleProjectile(const TSubclassOf<ASuraProjectile>& InProjectileClass, int32 NumPenetrable, bool bShouldConsumeAmmo, float AdditionalDamage, float AdditionalRecoilAmountPitch, float AdditionalRecoilAmountYaw, float AdditionalProjectileRadius, bool bIsHoming, AActor* HomingTarget)
{
	if (CurrentState != UnequippedState)
	{
		if (Character == nullptr || Character->GetController() == nullptr)
		{
			return;
		}

		if (bShouldConsumeAmmo)
		{
			if (!HasAmmoInCurrentMag())
			{
				return;
			}
		}

		FVector LineTraceStartLocation = Character->GetCamera()->GetComponentLocation();
		FVector LineTraceDirection = Character->GetCamera()->GetForwardVector();

		if (bIsZoomIn)
		{
			if (ZoomSpread.bEnableProjectileSpread)
			{
				LineTraceDirection = GetRandomSpreadVector(Character->GetCamera()->GetForwardVector());
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
				LineTraceDirection = GetRandomSpreadVector(Character->GetCamera()->GetForwardVector());
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
		if (InProjectileClass != nullptr)
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
				ASuraProjectile* Projectile = World->SpawnActor<ASuraProjectile>(InProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
				Projectile->InitializeProjectile(Character, this, AdditionalDamage, AdditionalProjectileRadius, NumPenetrable);
				SetUpAimUIDelegateBinding(Projectile);
				if (bIsHoming)
				{
					Projectile->SetHomingTarget(bIsHoming, HomingTarget);
				}
				Projectile->LaunchProjectile();

				SpawnMuzzleFireEffect(SpawnLocation, SpawnRotation);
			}
		}

		// Try and play the sound if specified
		if (FireSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
		}


		StartFireAnimation(AM_Fire_Character, AM_Fire_Weapon);
		if (bShouldConsumeAmmo)
		{
			ConsumeAmmo();
		}

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
			AddRecoilValue(&DefaultRecoil, AdditionalRecoilAmountPitch, AdditionalRecoilAmountYaw);
			ApplyCameraShake(DefaultCameraShakeClass);
		}
	}
}

void AWeapon::FireMultiProjectile(const TSubclassOf<ASuraProjectile>& InProjectileClass, int32 NumPenetrable, bool bShouldConsumeAmmo, float AdditionalDamage, float AdditionalRecoilAmountPitch, float AdditionalRecoilAmountYaw, float AdditionalProjectileRadius, bool bIsHoming, AActor* HomingTarget)
{
	if (CurrentState != UnequippedState)
	{
		if (Character == nullptr || Character->GetController() == nullptr)
		{
			return;
		}

		if (HasAmmoInCurrentMag())
		{
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

			FVector LineTraceStartLocation = Character->GetCamera()->GetComponentLocation();
			FVector LineTraceDirection = Character->GetCamera()->GetForwardVector();
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
			if (InProjectileClass != nullptr)
			{
				UWorld* const World = GetWorld();
				if (World != nullptr)
				{
					const FVector SpawnLocation = WeaponMesh->GetSocketLocation(FName(TEXT("Muzzle")));

					for (int pellet = 0; pellet < PelletsNum; pellet++)
					{
						const FRotator SpawnRotation = UKismetMathLibrary::RandomUnitVectorInConeInDegrees((TargetLocationOfProjectile - SpawnLocation).GetSafeNormal(), MaxAngleOfMultiProjectileSpread).Rotation();

						FActorSpawnParameters ActorSpawnParams;
						ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

						ASuraProjectile* Projectile = World->SpawnActor<ASuraProjectile>(InProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
						Projectile->InitializeProjectile(Character, this, AdditionalDamage, AdditionalProjectileRadius, NumPenetrable);
						SetUpAimUIDelegateBinding(Projectile);
						Projectile->LaunchProjectile();

						//TODO: ���߿��� Multi Homing Projectile�� �����ϰ� ���鿹��. �ڵ� Ÿ��������
					}

					SpawnMuzzleFireEffect(SpawnLocation, (TargetLocationOfProjectile - SpawnLocation).GetSafeNormal().Rotation());
				}
			}

			// Try and play the sound if specified
			if (FireSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
			}

			StartFireAnimation(AM_Fire_Character, AM_Fire_Weapon);

			if (bShouldConsumeAmmo)
			{
				ConsumeAmmo();
			}

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
				AddRecoilValue(&DefaultRecoil, AdditionalRecoilAmountPitch, AdditionalRecoilAmountYaw);
				ApplyCameraShake(DefaultCameraShakeClass);
			}
		}
	}
}

void AWeapon::SpawnProjectile()
{
	//TODO: Weapon name�� ���� �ٸ� Projectile�� spawn �ϵ��� �Ϸ��� �ߴµ�,
	//�׳� BP���� �ʱ⿡ Projectile Ŭ������ �������ָ� �Ǵ� ���̿��� �ϴ��� ��������.
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
	if (Character->GetWeaponSystem() && Character->GetWeaponSystem()->GetClass()->ImplementsInterface(UWeaponInterface::StaticClass()))
	{
		Character->GetWeaponSystem()->ZoomIn(true);
		StartCameraSettingChange(&CamSetting_ZoomIn);
	}
	ActivateCrosshairWidget(false);
}

void AWeapon::ZoomOut()
{
	bIsZoomIn = false;

	if (Character->GetWeaponSystem() && Character->GetWeaponSystem()->GetClass()->ImplementsInterface(UWeaponInterface::StaticClass()))
	{
		Character->GetWeaponSystem()->ZoomIn(false);
		StartCameraSettingChange(&CamSetting_Default);
	}
	ActivateCrosshairWidget(true);
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

void AWeapon::SetWeaponData(FWeaponData* InWeaponData)
{
	if (!InWeaponData) return;

	// 원본 데이터 참조 저장
	WeaponData = InWeaponData;

	if (WeaponData)
	{
		// <Action>
		LeftMouseAction = WeaponData->LeftMouseAction;
		RightMouseAction = WeaponData->RightMouseAction;

		// <Projectile>
		LeftProjectileClass = WeaponData->LeftProjectileClass;
		RightProjectileClass = WeaponData->RightProjectileClass;

		// <Sound>
		FireSound = WeaponData->FireSound;
		ChargeSound = WeaponData->ChargeSound;

		// <Effect>
		MuzzleFireEffect = WeaponData->FireEffect;
		ChargeEffect = WeaponData->ChargeEffect;
		ChargeEffectLocation = WeaponData->ChargeEffectLocation;
		ChargeEffectRotation = WeaponData->ChargeEffectRotation;
		ChargeEffenctScale = WeaponData->ChargeEffenctScale;

		// <Reload>
		MaxTotalAmmo = WeaponData->MaxTotalAmmo;
		TotalAmmo = MaxTotalAmmo;
		MaxAmmoPerMag = WeaponData->MaxAmmoPerMag;
		LeftAmmoInCurrentMag = MaxAmmoPerMag;

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
		bEnableMultiProjectile_Left = WeaponData->bEnableMultiProjectile_Left;
		bEnableMultiProjectile_Right = WeaponData->bEnableMultiProjectile_Right;
		MaxAngleOfMultiProjectileSpread = WeaponData->MaxAngleOfMultiProjectileSpread;

		// <Recoil>
		DefaultRecoil = WeaponData->DefaultRecoil;
		ZoomRecoil = WeaponData->ZoomRecoil;

		// <Camera Shake>
		DefaultCameraShakeClass = WeaponData->DefaultCameraShakeClass;
		ZoomCameraShakeClass = WeaponData->ZoomCameraShakeClass;
		ChargingCameraShakeClass = WeaponData->ChargingCameraShakeClass;

		// <Targeting(Homing)>
		MissileLaunchDelay = WeaponData->MissileLaunchDelay;
		MaxTargetNum = WeaponData->MaxTargetNum;
		MaxTargetDetectionRadius = WeaponData->MaxTargetDetectionRadius;
		MaxTargetDetectionAngle = WeaponData->MaxTargetDetectionAngle;
		MaxTargetDetectionTime = WeaponData->MaxTargetDetectionTime;
		TimeToReachMaxTargetDetectionRange = WeaponData->TimeToReachMaxTargetDetectionRange;

		// <Charging>
		bAutoFireAtMaxChargeTime = WeaponData->bAutoFireAtMaxChargeTime;
		ChargeTimeThreshold = WeaponData->ChargeTimeThreshold;
		MaxChargeTime = WeaponData->MaxChargeTime;
		ChargingAdditionalDamageBase = WeaponData->ChargingAdditionalDamageBase;
		ChargingAdditionalRecoilAmountPitchBase = WeaponData->ChargingAdditionalRecoilAmountPitchBase;
		ChargingAdditionalRecoilAmountYawBase = WeaponData->ChargingAdditionalRecoilAmountYawBase;
		ChargingAdditionalProjectileRadiusBase = WeaponData->ChargingAdditionalProjectileRadiusBase;

		// <Penetration>
		MaxPenetrableObjectsNum_Left = WeaponData->MaxPenetrableObjectsNum_Left;
		MaxPenetrableObjectsNum_Right = WeaponData->MaxPenetrableObjectsNum_Right;
	}
}

void AWeapon::SetCharacter(class ASuraCharacterPlayerWeapon* InCharacter)
{
	if (!InCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("SetCharacter 호출: 전달된 캐릭터가 null입니다."));
		return;
	}

	Character = InCharacter;
	UE_LOG(LogTemp, Log, TEXT("SetCharacter 완료: %s에 할당됨"), *Character->GetName());
}

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
void AWeapon::StartAnimation(UAnimMontage* CharacterAnimation, UAnimMontage* WeaponAnimation, float CharacterAnimPlayRate, float WeaponAnimPlayRate)
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
	
			UE_LOG(LogTemp, Warning, TEXT("Reloading Weapon Animation!!!"));
			CharacterAnimInstance->Montage_Play(CharacterAnimation, CharacterAnimation->GetPlayLength() / CharacterAnimPlayRate);
		}
	}

	if (WeaponAnimInstance != nullptr && WeaponAnimation != nullptr)
	{
		if (!WeaponMesh->GetAnimInstance()->Montage_IsPlaying(WeaponAnimation))
		{
			WeaponMesh->GetAnimInstance()->Montage_Play(WeaponAnimation, WeaponAnimation->GetPlayLength() / WeaponAnimPlayRate);
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


void AWeapon::PlayChargeSound()
{
	if (ChargeSound)
	{
		ChargeAudioComponent = UGameplayStatics::SpawnSoundAttached(ChargeSound, GetRootComponent(), FName(TEXT("Muzzle")), FVector(0, 0, 0), EAttachLocation::KeepRelativeOffset);
	}
}
void AWeapon::StopChargeSound()
{
	if (ChargeAudioComponent)
	{
		ChargeAudioComponent->Stop();
	}
}

#pragma region Niagara
void AWeapon::SpawnMuzzleFireEffect(FVector SpawnLocation, FRotator SpawnRotation)
{
	if (MuzzleFireEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
			GetWorld(), 
			MuzzleFireEffect,
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
void AWeapon::SwitchWeapon(ASuraCharacterPlayerWeapon* TargetCharacter, bool bEquip)
{
	//TODO: Reloading ���̿��ٸ�, CancelReload �������
	if (CurrentState == ReloadingState)
	{
		CancelReload();
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
	}
}
void AWeapon::EndWeaponSwitch(ASuraCharacterPlayerWeapon* TargetCharacter, bool bEquip)
{
	if (bEquip)
	{
		EquipWeapon(TargetCharacter);
	}
	else
	{
		UnequipWeapon(TargetCharacter);
		//TODO: �̰� ���� Interface�� ó���߾�� �߳�? �ٸ� ����� �� ���� �� ����
		if (TargetCharacter && TargetCharacter->GetWeaponSystem()
			&& TargetCharacter->GetWeaponSystem()->GetClass()->ImplementsInterface(UWeaponInterface::StaticClass()))
		{
			Cast<IWeaponInterface>(TargetCharacter->GetWeaponSystem())->SwitchToOtherWeapon();
		}
	}
}
void AWeapon::EquipWeapon(ASuraCharacterPlayerWeapon* TargetCharacter)
{
	UE_LOG(LogTemp, Warning, TEXT("Equip Weapon!!!"));

	SetInputActionBinding();

	ChangeState(IdleState);
}

void AWeapon::UnequipWeapon(ASuraCharacterPlayerWeapon* TargetCharacter)
{
	ResetInputActionBinding();
	DetachWeaponFromPlayer();

	UE_LOG(LogTemp, Warning, TEXT("Unequip Weapon!!!"));
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
						true, !bEnableMultiProjectile_Left, MaxPenetrableObjectsNum_Left
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
						true, !bEnableMultiProjectile_Left, MaxPenetrableObjectsNum_Left
					));
				}
				else if (LeftMouseAction == EWeaponAction::WeaponAction_FullAutoShot)
				{
					InputActionBindingHandles.Add(&EnhancedInputComponent->BindActionValueLambda(
						LeftFullAutoShotAction,
						ETriggerEvent::Started,
						[this](const FInputActionValue& InputActionValue, bool bIsLeftInput, bool bSingleProjectile, int32 NumPenetrable)
						{
							StartFullAutoShot(bIsLeftInput, bSingleProjectile, NumPenetrable);
						},
						true, !bEnableMultiProjectile_Left, MaxPenetrableObjectsNum_Left
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
						[this](const FInputActionValue& InputActionValue, const TSubclassOf<ASuraProjectile>& InProjectileClass)
						{
							StopTargetDetection(InProjectileClass);
						},
						LeftProjectileClass
					));
				}
				else if (LeftMouseAction == EWeaponAction::WeaponAction_Zoom)
				{
					// No Left Zoom
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
						false, !bEnableMultiProjectile_Right, MaxPenetrableObjectsNum_Right
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
						false, !bEnableMultiProjectile_Right, MaxPenetrableObjectsNum_Right
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
						false, !bEnableMultiProjectile_Right, MaxPenetrableObjectsNum_Right
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
						[this](const FInputActionValue& InputActionValue, const TSubclassOf<ASuraProjectile>& InProjectileClass)
						{
							StopTargetDetection(InProjectileClass);
						},
						RightProjectileClass
					));
				}
				else if (RightMouseAction == EWeaponAction::WeaponAction_Zoom)
				{
					InputActionBindingHandles.Add(&EnhancedInputComponent->BindAction(RightZoomAction, ETriggerEvent::Started, this, &AWeapon::ZoomToggle));
				}

				// <Reload>
				InputActionBindingHandles.Add(&EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &AWeapon::HandleReload));
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
void AWeapon::StopReload()
{
	ReloadAmmo();
	ChangeState(IdleState);
}

void AWeapon::ConsumeAmmo()
{
	if (LeftAmmoInCurrentMag > 0)
	{
		LeftAmmoInCurrentMag--;
		if (AmmoCounterWidget)
		{
			AmmoCounterWidget->UpdateAmmoCount(LeftAmmoInCurrentMag);
		}
	}
}
void AWeapon::ReloadAmmo()
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
bool AWeapon::HasAmmoInCurrentMag()
{
	return (LeftAmmoInCurrentMag > 0);
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
			ChangeState(ReloadingState);
		}
	}
}
void AWeapon::ReloadingEnd() //Legacy: ��� ����. animation�� ���� ���� ó���� ���� ���� ��.
{
	UE_LOG(LogTemp, Warning, TEXT("Reloading End!!!"));
}
#pragma endregion

void AWeapon::Create3DUI()
{
	////RenderTarget ����
	//RenderTarget = NewObject<UTextureRenderTarget2D>();
	//RenderTarget->InitAutoFormat(512, 512); // UI �ػ� ����
	//RenderTarget->ClearColor = FLinearColor::Transparent;
	//RenderTarget->UpdateResource();

	////UUserWidget�� RenderTarget�� �׸���
	//UUserWidget* AmmoCounterWidget = CreateWidget<UUserWidget>(GetWorld(), LoadClass<UUserWidget>(nullptr, TEXT("/Game/UI/AmmoCounterWidget_BP.AmmoCounterWidget_BP_C")));
	//if (AmmoCounterWidget)
	//{
	//	UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(
	//		GetWorld()->GetFirstPlayerController(),
	//		GetComponentLocation(),
	//		RenderTarget-,
	//		true
	//	);

	//	AmmoCounterWidget->AddToViewport();
	//}

	////��Ƽ���� ���� �ν��Ͻ� ���� �� RenderTarget ����
	//UMaterialInterface* BaseMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/Materials/M_UI_BaseMaterial.M_UI_BaseMaterial"));
	//if (BaseMaterial)
	//{
	//	WidgetMaterialInstance = UMaterialInstanceDynamic::Create(BaseMaterial, this);
	//	WidgetMaterialInstance->SetTextureParameterValue(FName("RenderTargetTexture"), RenderTarget);
	//	SetMaterial(0, WidgetMaterialInstance);
	//}
}

void AWeapon::ActivateCrosshairWidget(bool bflag)
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

void AWeapon::SetUpAimUIDelegateBinding(ASuraProjectile* Projectile)
{
	if (AimUIWidget)
	{
		AimUIWidget->SetUpAimUIDelegateBinding(Projectile);
	}
}

#pragma region FireMode
void AWeapon::HandleSingleFire(bool bIsLeftInput, bool bSingleProjectile, int32 NumPenetrable)
{
	if (CurrentState == IdleState)
	{
		ChangeState(FiringState);
		StartSingleShot(bIsLeftInput, bSingleProjectile, NumPenetrable);
	}
}
void AWeapon::HandleBurstFire(bool bIsLeftInput, bool bSingleProjectile, int32 NumPenetrable)
{
	if (CurrentState == IdleState)
	{
		ChangeState(FiringState);
		StartBurstFire(bIsLeftInput, bSingleProjectile, NumPenetrable);
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
void AWeapon::StartSingleShot(bool bIsLeftInput, bool bSingleProjectile, int32 NumPenetrable, float AdditionalDamage, float AdditionalRecoilAmountPitch, float AdditionalRecoilAmountYaw, float AdditionalProjectileRadius)
{
	if (bIsLeftInput)
	{
		if (bSingleProjectile)
		{
			FireSingleProjectile(LeftProjectileClass, NumPenetrable, true, AdditionalDamage, AdditionalRecoilAmountPitch, AdditionalRecoilAmountYaw, false);
		}
		else
		{
			FireMultiProjectile(LeftProjectileClass, NumPenetrable, true, AdditionalDamage, AdditionalRecoilAmountPitch, AdditionalRecoilAmountYaw, false);
		}
	}
	else
	{
		if (bSingleProjectile)
		{
			FireSingleProjectile(RightProjectileClass, NumPenetrable, true, AdditionalDamage, AdditionalRecoilAmountPitch, AdditionalRecoilAmountYaw, false);
		}
		else
		{
			FireMultiProjectile(RightProjectileClass, NumPenetrable, true, AdditionalDamage, AdditionalRecoilAmountPitch, AdditionalRecoilAmountYaw, false);
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
				FireSingleProjectile(LeftProjectileClass, NumPenetrable, true, AdditionalDamage, AdditionalRecoilAmountPitch, AdditionalRecoilAmountYaw, false);
			}
			else
			{
				FireMultiProjectile(LeftProjectileClass, NumPenetrable, true, AdditionalDamage, AdditionalRecoilAmountPitch, AdditionalRecoilAmountYaw, false);
			}
		}
		else
		{
			if (bSingleProjectile)
			{
				FireSingleProjectile(RightProjectileClass, NumPenetrable, true, AdditionalDamage, AdditionalRecoilAmountPitch, AdditionalRecoilAmountYaw, false);
			}
			else
			{
				FireMultiProjectile(RightProjectileClass, NumPenetrable, true, AdditionalDamage, AdditionalRecoilAmountPitch, AdditionalRecoilAmountYaw, false);
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

		UE_LOG(LogTemp, Warning, TEXT("FullAutoShot Started!!!"));
		if (!GetWorld()->GetTimerManager().IsTimerActive(FullAutoShotTimer))
		{
			if (bIsLeftInput)
			{
				if (bSingleProjectile)
				{
					FireSingleProjectile(LeftProjectileClass, NumPenetrable);
					int32 PenetrableNum = NumPenetrable;
					bool bShouldConsumeAmmo = true;
					float AdditionalDamage = 0.f;
					bool bCanHoming = false;
					GetWorld()->GetTimerManager().SetTimer(FullAutoShotTimer, [this, PenetrableNum, bShouldConsumeAmmo, AdditionalDamage, bCanHoming]() {FireSingleProjectile(LeftProjectileClass, PenetrableNum, bShouldConsumeAmmo, AdditionalDamage, bCanHoming); }, FullAutoShotFireRate, true);
				}
				else
				{
					FireMultiProjectile(LeftProjectileClass, NumPenetrable);
					int32 PenetrableNum = NumPenetrable;
					GetWorld()->GetTimerManager().SetTimer(FullAutoShotTimer, [this, PenetrableNum]() {FireMultiProjectile(LeftProjectileClass, PenetrableNum); }, FullAutoShotFireRate, true);
				}
			}
			else
			{
				if (bSingleProjectile)
				{
					FireSingleProjectile(RightProjectileClass, NumPenetrable);
					int32 PenetrableNum = NumPenetrable;
					bool bShouldConsumeAmmo = true;
					float AdditionalDamage = 0.f;
					bool bCanHoming = false;
					GetWorld()->GetTimerManager().SetTimer(FullAutoShotTimer, [this, PenetrableNum, bShouldConsumeAmmo, AdditionalDamage, bCanHoming]() {FireSingleProjectile(RightProjectileClass, PenetrableNum, bShouldConsumeAmmo, AdditionalDamage, bCanHoming); }, FullAutoShotFireRate, true);
				}
				else
				{
					FireMultiProjectile(RightProjectileClass, NumPenetrable);
					int32 PenetrableNum = NumPenetrable;
					GetWorld()->GetTimerManager().SetTimer(FullAutoShotTimer, [this, PenetrableNum]() {FireMultiProjectile(RightProjectileClass, PenetrableNum); }, FullAutoShotFireRate, true);
				}
			}
		}
	}
}
void AWeapon::StopFullAutoShot()
{
	//TODO: ��Ŀ� ������ �ȵ�. �ٸ� ��� �����غ���
	if (CurrentState == FiringState)
	{
		UE_LOG(LogTemp, Warning, TEXT("FullAutoShot Ended!!!"));
		GetWorld()->GetTimerManager().ClearTimer(FullAutoShotTimer);

		ChangeState(IdleState);
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
			UUserWidget** TargetMarkerPtr = MapTargetActorToWidget.Find(PreviousTarget);
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
						UUserWidget* NewTargetMarker = CreateTargetMarkerWidget(NewOverlappedActor);
						if (NewTargetMarker)
						{
							TargetMarkerWidgets.Add(NewTargetMarker);
							NewTargetMarker->AddToViewport();
							NewTargetMarker->SetAlignmentInViewport(FVector2D(0.5f, 0.5f));
							NewTargetMarker->SetVisibility(ESlateVisibility::Hidden);
						}
					}
				}
			}
		}
	}


	float DeltaSeconds = GetWorld()->GetDeltaSeconds();
	GetWorld()->GetTimerManager().SetTimer(TargetDetectionTimer, [this, DeltaSeconds]() {UpdateTargetDetection(DeltaSeconds); }, DeltaSeconds, false);
}
void AWeapon::StopTargetDetection(const TSubclassOf<ASuraProjectile>& InProjectileClass)
{
	if (CurrentState == TargetingState)
	{
		UE_LOG(LogTemp, Warning, TEXT("Stop Target Detection!!!"));

		GetWorld()->GetTimerManager().ClearTimer(TargetDetectionTimer);

		ElapsedTimeAfterTargetingStarted = 0.f;
		CurrentTargetDetectionRadius = 0.f;
		CurrentTargetDetectionAngle = 0.f;

		ResetTargetMarkers();

		TArray<AActor*> TargetsArray = Targets.Array();
		Targets.Empty();
		StartMissileLaunch(TargetsArray, InProjectileClass);
	}
}

bool AWeapon::SearchOverlappedActor(FVector CenterLocation, float SearchRadius, TArray<AActor*>& OverlappedActors)
{
	TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes;
	traceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
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
UUserWidget* AWeapon::CreateTargetMarkerWidget(AActor* TargetActor)
{
	if (TargetMarkerWidgetClass)
	{
		UUserWidget* NewTargetMarkerWidget = CreateWidget<UUserWidget>(GetWorld(), TargetMarkerWidgetClass);
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

		UUserWidget** TargetMarkerPtr = MapTargetActorToWidget.Find(Target);

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
		UUserWidget** TargetMarkerPtr = MapTargetActorToWidget.Find(Target);

		(*TargetMarkerPtr)->RemoveFromViewport();
		(*TargetMarkerPtr)->RemoveFromParent();
	}
	TargetMarkerWidgets.Empty();
	MapTargetActorToWidget.Empty();
}

void AWeapon::StartMissileLaunch(TArray<AActor*> TargetActors, const TSubclassOf<ASuraProjectile>& InProjectileClass)
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
		UpdateMissileLaunch(InProjectileClass);
	}
}
void AWeapon::UpdateMissileLaunch(const TSubclassOf<ASuraProjectile>& InProjectileClass)
{
	FireSingleProjectile(InProjectileClass, 0, false, 0.f, 0.f, 0.f, 0.f, true, ConfirmedTargets[CurrentTargetIndex]);
	CurrentTargetIndex++;
	if (ConfirmedTargets.Num() <= CurrentTargetIndex)
	{
		StopMissileLaunch();
	}
	else
	{
		//GetWorld()->GetTimerManager().SetTimer(MissileLaunchTimer, this, &AWeapon::UpdateMissileLaunch, MissileLaunchDelay, false);
		GetWorld()->GetTimerManager().SetTimer(MissileLaunchTimer, [this, InProjectileClass] {UpdateMissileLaunch(InProjectileClass); }, MissileLaunchDelay, false);
	}
}
void AWeapon::StopMissileLaunch()
{
	ConfirmedTargets.Empty();
	CurrentTargetIndex = 0;

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
		PlayChargeSound();

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
		StopChargeSound();

		ChangeState(FiringState);

		float ChargingAdditionalDamage = 0.f;
		float AdditionalRecoilAmountPitch = 0.f;
		float AdditionalRecoilAmountYaw = 0.f;
		float AdditionalProjectileRadius = 0.f;
		int32 PenetrableObjectsNum = 0;
		if (ElapsedChargeTime > ChargeTimeThreshold)
		{
			// TODO: Clamp �������...(���ص� �ɵ�?)
			ChargingAdditionalDamage = ((ElapsedChargeTime - ChargeTimeThreshold) / (MaxChargeTime - ChargeTimeThreshold)) * ChargingAdditionalDamageBase;
			AdditionalRecoilAmountPitch = ((ElapsedChargeTime - ChargeTimeThreshold) / (MaxChargeTime - ChargeTimeThreshold)) * ChargingAdditionalRecoilAmountPitchBase;
			AdditionalRecoilAmountYaw = ((ElapsedChargeTime - ChargeTimeThreshold) / (MaxChargeTime - ChargeTimeThreshold)) * ChargingAdditionalRecoilAmountYawBase;
			AdditionalProjectileRadius = ((ElapsedChargeTime - ChargeTimeThreshold) / (MaxChargeTime - ChargeTimeThreshold)) * ChargingAdditionalProjectileRadiusBase;
			PenetrableObjectsNum = ((ElapsedChargeTime - ChargeTimeThreshold) / (MaxChargeTime - ChargeTimeThreshold)) * MaxPenetrableObjectsNum_Left; //TODO: ������ ���� �޸� �ؾ���
			UE_LOG(LogTemp, Error, TEXT("Penetrable Num: %d"), PenetrableObjectsNum);
		}

		//TODO: Charge�� Ŀ����ȭ �ϴϱ� �� �ڵ嵵 �޸� ǥ���ؾ���
		StartSingleShot(true, true, PenetrableObjectsNum, ChargingAdditionalDamage, AdditionalRecoilAmountPitch, AdditionalRecoilAmountYaw, AdditionalProjectileRadius);

		ElapsedChargeTime = 0.f;
	}
}
#pragma endregion

#pragma region Recoil
void AWeapon::AddRecoilValue(FWeaponRecoilStruct* RecoilStruct, float AdditionalRecoilAmountPitch, float AdditionalRecoilAmountYaw)
{
	bIsRecoiling = true;

	if (RecoilStruct)
	{
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
		float InterpRecoilRecoverTargetValue_Yaw = FMath::FInterpTo(0.f, CulmulatedRecoilValueYaw - RecoveredRecoilValueYaw, DeltaTime, RecoilStruct->RecoilRecoverSpeed);;

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
				ApplyRecoil(DeltaTime, &DefaultRecoil);
				RecoverRecoil(DeltaTime, &DefaultRecoil);
			}
		}
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

	UpdateCameraSetting(GetWorld()->GetDeltaSeconds(), CamSetting);
}
void AWeapon::UpdateCameraSetting(float DeltaTime, FWeaponCamSettingValue* CamSetting)
{
	if (Character)
	{
		UCameraComponent* Camera = Character->GetCamera();
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

