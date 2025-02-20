
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
UACWeapon::UACWeapon()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true; //TODO: Tick Ȱ���ϸ� Ȱ��ȭ�ؾ���.

	WeaponAnimInstance = GetAnimInstance();

	//TODO: Muzzle Offset�� weapon Type�� ���� �ٸ���
	// ����� �⺻������ Weapon Mesh��"Muzzle" Socket�� ��ġ�� �̿��ϰ� �־ MuzzleOffset�� ��� ���ϴ� ����
	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);

	// Fire Mode
	//TODO: �ϴ��� �⺻ Single�� �ϴµ�, WeaponName�� ���� �����ڿ��� �������ִ� ������ �����ϱ�
	WeaponName = EWeaponName::WeaponName_Rifle;
	WeaponType = EWeaponType::WeaponType_Rifle;
	WeaponFireMode = EWeaponFireMode::WeaponFireMode_Single;
	
	SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetCollisionObjectType(ECC_GameTraceChannel3); //Weapon
	SetCollisionResponseToAllChannels(ECR_Ignore);

	//---------------------------------------------------------------------------------
	NumOfLeftAmmo = MaxAmmo;

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

void UACWeapon::InitializeWeapon(ASuraCharacterPlayerWeapon* NewCharacter)
{
	Character = NewCharacter;
	if (Character)
	{
		CharacterAnimInstance = Character->GetArmMesh()->GetAnimInstance();
		InitializeCamera(Character);

		if (WeaponName == EWeaponName::WeaponName_Rifle)
		{
			LoadWeaponData("Rifle");
		}
		else if (WeaponName == EWeaponName::WeaponName_ShotGun)
		{
			LoadWeaponData("ShotGun");
		}
		else if (WeaponName == EWeaponName::WeaponName_MissileLauncher)
		{
			LoadWeaponData("MissileLauncher");
		}
		else if (WeaponName == EWeaponName::WeaponName_RailGun)
		{
			LoadWeaponData("RailGun");
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

	// Set Aim Socket Relative Transform
	SetAimSocketRelativeTransform();
}

void UACWeapon::InitializeCamera(ASuraCharacterPlayerWeapon* NewCharacter)
{
	if (NewCharacter)
	{
		CamSetting_Default.CameraRelativeLocation = NewCharacter->GetCamera()->GetRelativeLocation();
		CamSetting_Default.CameraRelativeRotation = NewCharacter->GetCamera()->GetRelativeRotation();
		CamSetting_Default.FOV = NewCharacter->GetCamera()->FieldOfView;
	}
}

void UACWeapon::InitializeUI()
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
			AmmoCounterWidget->UpdateAmmoCount(NumOfLeftAmmo);
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

void UACWeapon::LoadWeaponData(FName WeaponID)
{
	WeaponData = WeaponDataTable->FindRow<FWeaponData>(WeaponID, TEXT(""));
	if (WeaponData)
	{
		//TODO: The data you load must be different depending on the weapon type

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
		MaxAmmo = WeaponData->MaxAmmo;
		NumOfLeftAmmo = MaxAmmo;

		// <SingleShot>
		SingleShotDelay = WeaponData->SingleShotDelay;

		// <BurstShot>
		BurstShotFireRate = WeaponData->BurstShotFireRate;
		BurstShotCount = WeaponData->BurstShotCount;

		// <FullAutoShot>
		FullAutoShotFireRate = WeaponData->FullAutoShotFireRate;

		// <ProjectileSpread>
		DefaultSpread = WeaponData->DefaultSpread;
		ZoomSpread = WeaponData->ZoomSpread;

		// <MultiProjectileSpread>
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
		MaxPenetrableObjectsNum = WeaponData->MaxPenetrableObjectsNum;
	}
}

// Called when the game starts
void UACWeapon::BeginPlay()
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

	WeaponAnimInstance = GetAnimInstance();

	CurrentState = UnequippedState;

	//TODO: BeginPlay���� ChangeState�� ����ϸ�, ChangeState -> EnterState -> ���⿡ ���� ���� �� ������ �߻���
	// �������� ������ �� ������, ���� ��Ȯ�� ������ ã�� ������
	//ChangeState(UnequippedState);


	//SetCollisionObjectType(ECC_EngineTraceChannel2);

	//SetCollisionProfileName(FName("Weapon"));
	//SetCollisionResponseToAllChannels(ECR_Ignore);
}

void UACWeapon::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CurrentState)
	{
		CurrentState->UpdateState(this, DeltaTime);
	}
	UpdateRecoil(DeltaTime);

	UpdateSpread(DeltaTime);
}

void UACWeapon::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

bool UACWeapon::AttachWeaponToPlayer(ASuraCharacterPlayerWeapon* TargetCharacter)
{
	Character = TargetCharacter;

	//TODO: ���� ���� Ȯ�� ��� �����ϱ�. 
	// �׸��� WeaponSystemComponent���� weapon Inventory�� �ش� ���Ⱑ �����ϴ��� Ȯ���ؾ���
	// Check that the character is valid, and has no weapon component yet
	//if (Character == nullptr || Character->GetInstanceComponents().FindItemByClass<UACWeapon>())
	//{
	//	return false;
	//}
	if (Character == nullptr)
	{
		return false;
	}

	// Attach the weapon to the First Person Character
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);

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
	RightHandToAimSocketOffset = this->GetSocketLocation(FName(TEXT("Aim"))) - Character->GetArmMesh()->GetBoneLocation(FName(TEXT("hand_r")));
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

	SetVisibility(true);

	return true;
}

void UACWeapon::DetachWeaponFromPlayer()
{
	if (Character == nullptr)
	{
		return;
	}
	else
	{
		DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		//ActivateCrosshairWidget(false);
		SetVisibility(false);
	}
}

void UACWeapon::FireSingleProjectile(bool bShouldConsumeAmmo, float AdditionalDamage, float AdditionalRecoilAmountPitch, float AdditionalRecoilAmountYaw, float AdditionalProjectileRadius, int32 NumPenetrable, bool bIsHoming, AActor* HomingTarget)
{
	//TODO: Projectile ������ ���� ������ input���� �޾ƾ� �ϳ�? ������ �� �𸣁���
	if (CurrentState != UnequippedState)
	{
		if (Character == nullptr || Character->GetController() == nullptr)
		{
			return;
		}

		if (bShouldConsumeAmmo)
		{
			if (!HasAmmo())
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
		if (ProjectileClass != nullptr)
		{
			UWorld* const World = GetWorld();
			if (World != nullptr)
			{
				const FVector SpawnLocation = this->GetSocketLocation(FName(TEXT("Muzzle")));
				const FRotator SpawnRotation = (TargetLocationOfProjectile - SpawnLocation).Rotation();

				//Set Spawn Collision Handling Override
				FActorSpawnParameters ActorSpawnParams;
				ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				// Spawn the projectile at the muzzle
				ASuraProjectile* Projectile = World->SpawnActor<ASuraProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
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

void UACWeapon::FireMultiProjectile()
{
	if (CurrentState != UnequippedState)
	{
		if (Character == nullptr || Character->GetController() == nullptr)
		{
			return;
		}

		if (HasAmmo())
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
			if (ProjectileClass != nullptr)
			{
				UWorld* const World = GetWorld();
				if (World != nullptr)
				{
					const FVector SpawnLocation = this->GetSocketLocation(FName(TEXT("Muzzle")));

					for (int pellet = 0; pellet < PelletsNum; pellet++)
					{
						const FRotator SpawnRotation = UKismetMathLibrary::RandomUnitVectorInConeInDegrees((TargetLocationOfProjectile - SpawnLocation).GetSafeNormal(), MaxAngleOfMultiProjectileSpread).Rotation();

						FActorSpawnParameters ActorSpawnParams;
						ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

						ASuraProjectile* Projectile = World->SpawnActor<ASuraProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
						Projectile->InitializeProjectile(Character, this);
						SetUpAimUIDelegateBinding(Projectile);
						Projectile->LaunchProjectile();
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

			ConsumeAmmo();

			// <Recoil & CamShake>
			if (bIsZoomIn)
			{
				AddRecoilValue(&ZoomRecoil);
				ApplyCameraShake(ZoomCameraShakeClass);
			}
			else
			{
				AddRecoilValue(&DefaultRecoil);
				ApplyCameraShake(DefaultCameraShakeClass);
			}
		}
	}
}

void UACWeapon::SpawnProjectile()
{
	//TODO: Weapon name�� ���� �ٸ� Projectile�� spawn �ϵ��� �Ϸ��� �ߴµ�,
	//�׳� BP���� �ʱ⿡ Projectile Ŭ������ �������ָ� �Ǵ� ���̿��� �ϴ��� ��������.

}

void UACWeapon::ZoomToggle()
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

void UACWeapon::ZoomIn()
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

void UACWeapon::ZoomOut()
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
void UACWeapon::ChangeState(USuraWeaponBaseState* NewState)
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
void UACWeapon::StartFireAnimation(UAnimMontage* CharacterFireAnimation, UAnimMontage* WeaponFireAnimation)
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
		if (!GetAnimInstance()->Montage_IsPlaying(WeaponFireAnimation))
		{
			GetAnimInstance()->Montage_Play(WeaponFireAnimation, 1.f);
		}
	}
}
void UACWeapon::StartAnimation(UAnimMontage* CharacterAnimation, UAnimMontage* WeaponAnimation, float CharacterAnimPlayRate, float WeaponAnimPlayRate)
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
		if (!GetAnimInstance()->Montage_IsPlaying(WeaponAnimation))
		{
			GetAnimInstance()->Montage_Play(WeaponAnimation, WeaponAnimation->GetPlayLength() / WeaponAnimPlayRate);
		}
	}
}
void UACWeapon::CancelAnimation(UAnimMontage* CharacterAnimation, UAnimMontage* WeaponAnimation)
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
		if (GetAnimInstance()->Montage_IsPlaying(WeaponAnimation))
		{
			GetAnimInstance()->Montage_Stop(0.f, WeaponAnimation);
		}
	}
}
#pragma endregion


void UACWeapon::PlayChargeSound()
{
	if (ChargeSound)
	{
		ChargeAudioComponent = UGameplayStatics::SpawnSoundAttached(ChargeSound, this, FName(TEXT("Muzzle")), FVector(0, 0, 0), EAttachLocation::KeepRelativeOffset);
	}
}

void UACWeapon::StopChargeSound()
{
	if (ChargeAudioComponent)
	{
		ChargeAudioComponent->Stop();
	}
}

#pragma region Niagara
void UACWeapon::SpawnMuzzleFireEffect(FVector SpawnLocation, FRotator SpawnRotation)
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
void UACWeapon::SpawnChargeEffect(FVector SpawnLocation, FRotator SpawnRotation, FVector EffectScale)
{
	if (ChargeEffect)
	{
		ChargeEffectComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
			ChargeEffect,
			this,
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
void UACWeapon::DestroyChargeEffect()
{
	if (ChargeEffectComponent)
	{
		ChargeEffectComponent->Deactivate();
		ChargeEffectComponent->DestroyComponent();
		ChargeEffectComponent = nullptr;
	}
}
#pragma endregion


bool UACWeapon::PerformLineTrace(FVector StartLocation, FVector LineDirection, float MaxDistance, FVector& HitLocation)
{
	FVector Start = StartLocation;
	FVector End = StartLocation + LineDirection * MaxDistance;

	FHitResult HitResult;

	FCollisionQueryParams Params;
	Params.AddIgnoredComponent(this);
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

bool UACWeapon::PerformSphereTrace(FVector StartLocation, FVector TraceDirection, float MaxDistance, float SphereRadius, FVector& HitLocation)
{
	FVector Start = StartLocation;
	FVector End = StartLocation + TraceDirection * MaxDistance;

	FHitResult HitResult;

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Visibility);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_PhysicsBody);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

	FCollisionQueryParams Params;
	Params.AddIgnoredComponent(this);
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

FVector UACWeapon::CalculateScreenCenterWorldPositionAndDirection(FVector& OutWorldPosition, FVector& OutWorldDirection) const
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

void UACWeapon::SetAimSocketTransform()
{
	FTransform AimSocketTransform = GetSocketTransform(FName(TEXT("Aim")));
	//FTransform IKHandGunTransform = Character->GetMesh()->GetSocketTransform(FName(TEXT("ik_hand_gun")));
	FTransform IKHandGunTransform = Character->GetArmMesh()->GetSocketTransform(FName(TEXT("ik_hand_gun")));

	AimSocketRelativeTransform = AimSocketTransform.GetRelativeTransform(IKHandGunTransform);
}

void UACWeapon::SetAimSocketRelativeTransform()
{
	AimSocketRelativeTransform = GetSocketTransform(FName(TEXT("Aim")), ERelativeTransformSpace::RTS_Component);
}

FTransform UACWeapon::GetAimSocketRelativeTransform()
{
	return AimSocketRelativeTransform;
}

#pragma region Equip/Unequip
void UACWeapon::SwitchWeapon(ASuraCharacterPlayerWeapon* TargetCharacter, bool bEquip)
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
void UACWeapon::EndWeaponSwitch(ASuraCharacterPlayerWeapon* TargetCharacter, bool bEquip)
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
void UACWeapon::EquipWeapon(ASuraCharacterPlayerWeapon* TargetCharacter)
{
	// TODO: �Լ� ����ȭ �ؾ���
	UE_LOG(LogTemp, Warning, TEXT("Equip Weapon!!!"));
	//AttachWeaponToPlayer(TargetCharacter);
	SetInputActionBinding();

	ChangeState(IdleState);
}

void UACWeapon::UnequipWeapon(ASuraCharacterPlayerWeapon* TargetCharacter)
{
	// TODO: Detach ���� ó���ؾ��ϴ� �͵� ó���ؾ���
	ResetInputActionBinding();
	DetachWeaponFromPlayer();

	UE_LOG(LogTemp, Warning, TEXT("Unequip Weapon!!!"));
	ChangeState(UnequippedState);
}

void UACWeapon::SetInputActionBinding()
{
	if (Character)
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
		{
			if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
			{
				if (WeaponName == EWeaponName::WeaponName_Rifle)
				{
					//FullAuto Shot
					InputActionBindingHandles.Add(&EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &UACWeapon::StartFullAutoShot));
					InputActionBindingHandles.Add(&EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &UACWeapon::StopFullAutoShot));
					//Zoom
					InputActionBindingHandles.Add(&EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Started, this, &UACWeapon::ZoomToggle));
					//Reload
					InputActionBindingHandles.Add(&EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &UACWeapon::HandleReload));
				}
				else if (WeaponName == EWeaponName::WeaponName_ShotGun)
				{
					//Fire Single Shot
					InputActionBindingHandles.Add(&EnhancedInputComponent->BindAction(FireSingleShotAction, ETriggerEvent::Started, this, &UACWeapon::HandleSingleFire));
					InputActionBindingHandles.Add(&EnhancedInputComponent->BindAction(FireBurstShotAction, ETriggerEvent::Started, this, &UACWeapon::HandleBurstFire));
					//Reload
					InputActionBindingHandles.Add(&EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &UACWeapon::HandleReload));
				}
				else if (WeaponName == EWeaponName::WeaponName_MissileLauncher)
				{
					//Fire Single Shot
					InputActionBindingHandles.Add(&EnhancedInputComponent->BindAction(FireSingleShotAction, ETriggerEvent::Started, this, &UACWeapon::HandleSingleFire));
					//Targeting
					InputActionBindingHandles.Add(&EnhancedInputComponent->BindAction(HoldAction, ETriggerEvent::Triggered, this, &UACWeapon::StartTargetDetection));
					InputActionBindingHandles.Add(&EnhancedInputComponent->BindAction(HoldAction, ETriggerEvent::Completed, this, &UACWeapon::StopTargetDetection));
					//Reload
					InputActionBindingHandles.Add(&EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &UACWeapon::HandleReload));
				}
				else if (WeaponName == EWeaponName::WeaponName_RailGun)
				{
					//Charging
					InputActionBindingHandles.Add(&EnhancedInputComponent->BindAction(ChargeAction, ETriggerEvent::Triggered, this, &UACWeapon::StartCharge));
					InputActionBindingHandles.Add(&EnhancedInputComponent->BindAction(ChargeAction, ETriggerEvent::Completed, this, &UACWeapon::StopCharge));
					//Zoom
					InputActionBindingHandles.Add(&EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Started, this, &UACWeapon::ZoomToggle));
					//Reload
					InputActionBindingHandles.Add(&EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &UACWeapon::HandleReload));
				}
			}
		}
	}
}
void UACWeapon::ResetInputActionBinding()
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
void UACWeapon::HandleReload()
{
	if (CurrentState == IdleState)
	{
		if (NumOfLeftAmmo < MaxAmmo)
		{
			//TODO: ReloadingState�� EnterState���� StartReload �ص� �� ��
			ChangeState(ReloadingState);
			//StartReload();
		}
	}
}
void UACWeapon::CancelReload()
{
	CancelAnimation(AM_Reload_Character, AM_Reload_Weapon);
	GetWorld()->GetTimerManager().ClearTimer(ReloadingTimer);
}
void UACWeapon::StartReload()
{
	UE_LOG(LogTemp, Warning, TEXT("Start Reload!!!"));

	if (bIsZoomIn)
	{
		ZoomOut();
	}

	StartAnimation(AM_Reload_Character, AM_Reload_Weapon, ReloadingTime, ReloadingTime);
	GetWorld()->GetTimerManager().SetTimer(ReloadingTimer, this, &UACWeapon::StopReload, ReloadingTime, false);
}
void UACWeapon::StopReload()
{
	ReloadAmmo();
	ChangeState(IdleState);
}

void UACWeapon::ConsumeAmmo()
{
	if (NumOfLeftAmmo > 0)
	{
		NumOfLeftAmmo--;
		if (AmmoCounterWidget)
		{
			AmmoCounterWidget->UpdateAmmoCount(NumOfLeftAmmo);
		}
	}
}
void UACWeapon::ReloadAmmo()
{
	NumOfLeftAmmo = MaxAmmo;
	if (AmmoCounterWidget)
	{
		AmmoCounterWidget->UpdateAmmoCount(NumOfLeftAmmo);
	}
}
bool UACWeapon::HasAmmo()
{
	return (NumOfLeftAmmo > 0);
}
void UACWeapon::AutoReload()
{
	if (bCanAutoReload)
	{
		if (!HasAmmo())
		{
			ChangeState(ReloadingState);
		}
	}
}
void UACWeapon::ReloadingEnd() //Legacy: ��� ����. animation�� ���� ���� ó���� ���� ���� ��.
{
	UE_LOG(LogTemp, Warning, TEXT("Reloading End!!!"));
}
#pragma endregion

void UACWeapon::Create3DUI()
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

void UACWeapon::ActivateCrosshairWidget(bool bflag)
{
	//if (bflag)
	//{
	//	if (CrosshairWidget)
	//	{
	//		CrosshairWidget->AddToViewport();
	//	}
	//}
	//else
	//{
	//	if (CrosshairWidget)
	//	{
	//		CrosshairWidget->RemoveFromViewport();
	//	}
	//}

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
void UACWeapon::ActivateAmmoCounterWidget(bool bflag)
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

void UACWeapon::SetUpAimUIDelegateBinding(ASuraProjectile* Projectile)
{
	if (AimUIWidget)
	{
		AimUIWidget->SetUpAimUIDelegateBinding(Projectile);
	}
}

#pragma region FireMode
void UACWeapon::HandleSingleFire()
{
	if (CurrentState == IdleState)
	{
		ChangeState(FiringState);
		StartSingleShot();
	}
}
void UACWeapon::HandleBurstFire()
{
	if (CurrentState == IdleState)
	{
		ChangeState(FiringState);

		if (WeaponName == EWeaponName::WeaponName_Rifle)
		{
			StartBurstFire(false);
		}
		else if (WeaponName == EWeaponName::WeaponName_ShotGun)
		{
			StartBurstFire(true);
		}
	}
}
void UACWeapon::HandleFullAutoFire()
{
	if (CurrentState == IdleState)
	{
		ChangeState(FiringState);
		StartSingleShot();
	}
}
#pragma endregion

#pragma region FireMode/SingleShot
void UACWeapon::StartSingleShot(float AdditionalDamage, float AdditionalRecoilAmountPitch, float AdditionalRecoilAmountYaw, float AdditionalProjectileRadius, int32 NumPenetrable)
{
	//TODO: ����� �ٲ�� �� 
	//TODO: Enum�� ���� �ٸ��� �Լ� ȣ���ϴ� ����� �ƴ϶�, StartSingleShot ��ü�� Input�� �־ �ʿ��� ������ ���� �ٸ� Input �־ �۵��ϵ��� �ؾ���
	if (WeaponName == EWeaponName::WeaponName_Rifle)
	{
		FireSingleProjectile();
	}
	else if (WeaponName == EWeaponName::WeaponName_ShotGun)
	{
		FireMultiProjectile();
	}
	else if (WeaponName == EWeaponName::WeaponName_MissileLauncher)
	{
		FireSingleProjectile();
	}
	else if (WeaponName == EWeaponName::WeaponName_RailGun)
	{
		FireSingleProjectile(true, AdditionalDamage, AdditionalRecoilAmountPitch, AdditionalRecoilAmountYaw, false, NumPenetrable);
	}

	GetWorld()->GetTimerManager().SetTimer(SingleShotTimer, this, &UACWeapon::StopSingleShot, SingleShotDelay, false);
}
void UACWeapon::StopSingleShot()
{
	ChangeState(IdleState);
}
#pragma endregion

#pragma region FireMode/BurstShot
void UACWeapon::StartBurstFire(bool bMultiProjectile)
{
	if (BurstShotFired < BurstShotCount)
	{
		if (bMultiProjectile)
		{
			FireMultiProjectile();
		}
		else
		{
			FireSingleProjectile();
		}
		BurstShotFired++;
		GetWorld()->GetTimerManager().SetTimer(BurstShotTimer, [this, bMultiProjectile]() {StartBurstFire(bMultiProjectile); }, BurstShotFireRate, true);
	}
	else
	{
		StopBurstFire();
	}
}

void UACWeapon::StopBurstFire()
{
	BurstShotFired = 0;
	if (GetWorld()->GetTimerManager().IsTimerActive(BurstShotTimer))
	{
		GetWorld()->GetTimerManager().ClearTimer(BurstShotTimer);
	}

	ChangeState(IdleState);
}
#pragma endregion

#pragma region FireMode/FullAuto
void UACWeapon::StartFullAutoShot()
{
	if (CurrentState == IdleState)
	{
		ChangeState(FiringState);

		UE_LOG(LogTemp, Warning, TEXT("FullAutoShot Started!!!"));
		if (!GetWorld()->GetTimerManager().IsTimerActive(FullAutoShotTimer))
		{
			FireSingleProjectile();
			//GetWorld()->GetTimerManager().SetTimer(FullAutoShotTimer, this, &UACWeapon::FireSingleProjectile, FullAutoShotFireRate, true);
			bool bShouldConsumeAmmo = true;
			float AdditionalDamage = 0.f;
			bool bCanHoming = false;
			GetWorld()->GetTimerManager().SetTimer(FullAutoShotTimer, [this, bShouldConsumeAmmo, AdditionalDamage, bCanHoming]() {FireSingleProjectile(bShouldConsumeAmmo, AdditionalDamage, bCanHoming); }, FullAutoShotFireRate, true);
		}
	}
}
void UACWeapon::StopFullAutoShot()
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
void UACWeapon::StartTargetDetection()
{
	if (CurrentState == IdleState)
	{
		UE_LOG(LogTemp, Warning, TEXT("Start Target Detection!!!"));

		ChangeState(TargetingState);
		UpdateTargetDetection(GetWorld()->GetDeltaSeconds());
	}
}
void UACWeapon::UpdateTargetDetection(float DeltaTime) //TODO: �ش� Ÿ�� Ȥ�� ���� Ÿ���� ���� ���ε� �Ǵ��ؼ� Update �ؾ���
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
void UACWeapon::StopTargetDetection()
{
	UE_LOG(LogTemp, Warning, TEXT("Stop Target Detection!!!"));

	GetWorld()->GetTimerManager().ClearTimer(TargetDetectionTimer);

	ElapsedTimeAfterTargetingStarted = 0.f;
	CurrentTargetDetectionRadius = 0.f;
	CurrentTargetDetectionAngle = 0.f;

	ResetTargetMarkers();
	//TODO: Targets�� ����� Target�� ���� ���� �̻��� �߻� ����
	//�ϴ��� Targets�� �ʱ�ȭ ��Ű�⸸ ��
	//Targets.Empty();
	//MapTargetActorToWidget�� ��ҵ��� ���� �����ؾ��� -> �̷��� Widget�� �����ϴ� �����̳ʰ� �ʿ��� ��

	TArray<AActor*> TargetsArray = Targets.Array();
	Targets.Empty();
	StartMissileLaunch(TargetsArray);
}

bool UACWeapon::SearchOverlappedActor(FVector CenterLocation, float SearchRadius, TArray<AActor*>& OverlappedActors)
{
	TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes;
	traceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	TArray<AActor*> ignoreActors;
	ignoreActors.Init(Character, 1);
	bool bIsAnyActorExist = UKismetSystemLibrary::SphereOverlapActors(GetWorld(), CenterLocation, SearchRadius, traceObjectTypes, nullptr, ignoreActors, OverlappedActors);

	return bIsAnyActorExist;
}
TTuple<FVector2D, bool> UACWeapon::GetScreenPositionOfWorldLocation(const FVector& SearchLocation) const
{
	FVector2D ScreenLocation = FVector2D::ZeroVector;
	bool bResult = UGameplayStatics::ProjectWorldToScreen(CharacterController, SearchLocation, ScreenLocation);

	return MakeTuple(ScreenLocation, bResult);
}
bool UACWeapon::IsInViewport(FVector2D ActorScreenPosition, float ScreenRatio_Width, float ScreenRatio_Height) const
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
float UACWeapon::GetUnsignedAngleBetweenVectors(const FVector& VectorA, const FVector& VectorB, const FVector& Axis)
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
bool UACWeapon::CheckIfTargetIsBlockedByObstacle(AActor* target)
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
UUserWidget* UACWeapon::CreateTargetMarkerWidget(AActor* TargetActor)
{
	if (TargetMarkerWidgetClass)
	{
		UUserWidget* NewTargetMarkerWidget = CreateWidget<UUserWidget>(GetWorld(), TargetMarkerWidgetClass);
		MapTargetActorToWidget.Add(TargetActor, NewTargetMarkerWidget);
		return NewTargetMarkerWidget;
	}
	return nullptr;
}
void UACWeapon::UpdateTargetMarkers()
{
	for (AActor* Target : Targets)
	{
		FVector TargetLocation = Target->GetActorLocation();
		FVector TargetOffset(0.f, 0.f, 50.f);

		UUserWidget** TargetMarkerPtr = MapTargetActorToWidget.Find(Target);

		FVector2D TargetScreenPosition = GetScreenPositionOfWorldLocation(TargetLocation + TargetOffset).Get<0>();

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
void UACWeapon::ResetTargetMarkers()
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

void UACWeapon::StartMissileLaunch(TArray<AActor*> TargetActors)
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
		UpdateMissileLaunch();
	}
}
void UACWeapon::UpdateMissileLaunch()
{
	FireSingleProjectile(false, 0.f, 0.f, 0.f, 0.f, 0, true, ConfirmedTargets[CurrentTargetIndex]);
	CurrentTargetIndex++;
	if (ConfirmedTargets.Num() <= CurrentTargetIndex)
	{
		StopMissileLaunch();
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(MissileLaunchTimer, this, &UACWeapon::UpdateMissileLaunch, MissileLaunchDelay, false);
	}
}
void UACWeapon::StopMissileLaunch()
{
	ConfirmedTargets.Empty();
	CurrentTargetIndex = 0;

	ChangeState(IdleState);
}
#pragma endregion

#pragma region FireMode/Charging
void UACWeapon::StartCharge()
{
	//TODO: Chargning �ϴ� ���� CameraShake ����, Charging �ð��� ���� Camera Shake ������ ���� �� Ŀ���� �ϱ�
	if (CurrentState == IdleState)
	{
		UE_LOG(LogTemp, Warning, TEXT("Start Charging!!!"));

		ChangeState(ChargingState);

		SpawnChargeEffect(ChargeEffectLocation, ChargeEffectRotation, ChargeEffenctScale);
		PlayChargeSound();

		UpdateCharge();
	}

}
void UACWeapon::UpdateCharge()
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
			GetWorld()->GetTimerManager().SetTimer(ChargingTimer, this, &UACWeapon::UpdateCharge, DeltaSeconds, false);
		}
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(ChargingTimer, this, &UACWeapon::UpdateCharge, DeltaSeconds, false);
	}
}
void UACWeapon::StopCharge()
{
	UE_LOG(LogTemp, Warning, TEXT("Stop Charging!!!"));

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
			PenetrableObjectsNum = ((ElapsedChargeTime - ChargeTimeThreshold) / (MaxChargeTime - ChargeTimeThreshold)) * MaxPenetrableObjectsNum;
			UE_LOG(LogTemp, Error, TEXT("Penetrable Num: %d"), PenetrableObjectsNum);
		}

		StartSingleShot(ChargingAdditionalDamage, AdditionalRecoilAmountPitch, AdditionalRecoilAmountYaw, AdditionalProjectileRadius, PenetrableObjectsNum);

		ElapsedChargeTime = 0.f;
	}
}
#pragma endregion


#pragma region Recoil
void UACWeapon::AddRecoilValue(FWeaponRecoilStruct* RecoilStruct, float AdditionalRecoilAmountPitch, float AdditionalRecoilAmountYaw)
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

void UACWeapon::ApplyRecoil(float DeltaTime, FWeaponRecoilStruct* RecoilStruct)
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

void UACWeapon::RecoverRecoil(float DeltaTime, FWeaponRecoilStruct* RecoilStruct)
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
void UACWeapon::UpdateRecoil(float DeltaTime)
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

#pragma region Projectile/SingleProjectileSpread
void UACWeapon::AddSpreadValue(FProjectileSpreadValue* SpreadValue)
{
	if (!SpreadValue) return;

	bIsSpreading = true;
	SpreadRecoverTimer = 0.f;

	float RandSpreadValue = FMath::RandRange((SpreadValue->SpreadAmountBase) * SpreadValue->SpreadRangeMin, (SpreadValue->SpreadAmountBase) * SpreadValue->SpreadRangeMax);

	TotalTargetSpreadValue += RandSpreadValue;
	TotalTargetSpreadValue = FMath::Clamp(TotalTargetSpreadValue, 0.f, SpreadValue->MaxSpreadValue);
}
void UACWeapon::ApplySpread(float DeltaTime, FProjectileSpreadValue* SpreadValue)
{
	if (!SpreadValue) return;

	float InterpSpreadTargetValue = FMath::FInterpConstantTo(CurrentSpreadVaule, TotalTargetSpreadValue, DeltaTime, SpreadValue->SpreadSpeed);
	CurrentSpreadVaule = InterpSpreadTargetValue;

	SpreadRecoverTimer += DeltaTime;
}
void UACWeapon::RecoverSpread(float DeltaTime, FProjectileSpreadValue* SpreadValue)
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
void UACWeapon::UpdateSpread(float DeltaTime)
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
FVector UACWeapon::GetRandomSpreadVector(FVector BaseDir)
{
	const FVector SpreadVector = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(BaseDir.GetSafeNormal(), CurrentSpreadVaule);

	return SpreadVector;
}
#pragma endregion

#pragma region Camera
void UACWeapon::StartCameraSettingChange(FWeaponCamSettingValue* CamSetting)
{
	bIsUsingPlayerCamFov = true;

	if (GetWorld()->GetTimerManager().IsTimerActive(CamSettingTimer))
	{
		GetWorld()->GetTimerManager().ClearTimer(CamSettingTimer);
	}

	UpdateCameraSetting(GetWorld()->GetDeltaSeconds(), CamSetting);
}
void UACWeapon::UpdateCameraSetting(float DeltaTime, FWeaponCamSettingValue* CamSetting)
{
	//UE_LOG(LogTemp, Error, TEXT("UpdateCameraSetting"));

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
void UACWeapon::StopCameraSettingChange()
{
	bIsUsingPlayerCamFov = false;
	UE_LOG(LogTemp, Error, TEXT("Modifying Cam Setting is Completed!!!"));
}
void UACWeapon::ForceStopCamModification()
{
	bIsUsingPlayerCamFov = false;

	if (GetWorld()->GetTimerManager().IsTimerActive(CamSettingTimer))
	{
		GetWorld()->GetTimerManager().ClearTimer(CamSettingTimer);
	}
}
void UACWeapon::ApplyCameraShake(TSubclassOf<UWeaponCameraShakeBase> CamShakeClass, float Scale)
{
	if (Character && CamShakeClass)
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
		{
			//UE_LOG(LogTemp, Error, TEXT("CamShake!!!"));

			PlayerController->PlayerCameraManager->StartCameraShake(CamShakeClass, Scale);
		}
	}
}
#pragma endregion