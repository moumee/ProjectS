
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
#include "ActorComponents/WeaponSystem/WeaponCameraShakeBase.h"
#include "ActorComponents/WeaponSystem/AmmoCounterWidget.h"

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

#include "NiagaraComponent.h"
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
	}
	InitializeUI();

	// Set up action bindings
	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		//TODO: �Ʒ� �ڵ忡 ���ؼ� �˾ƺ�����. Multi Play�� ���� ���ΰ�?
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
			Subsystem->AddMappingContext(FireMappingContext, 1);
		}

		////TODO: Weapon Type�� ���� Bind�Ǵ� Action�� �ٸ����� �����ϱ�
		//if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		//{
		//	if (WeaponName == EWeaponName::WeaponName_Rifle)
		//	{
		//		// FullAuto Shot
		//		LeftMouseButtonActionBinding = &EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &UACWeapon::StartFullAutoShot);
		//		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &UACWeapon::StopFullAutoShot);

		//		//TODO: Zoom�� Holding ������� �ٲ����
		//		// Zoom Toggle
		//		RightMouseButtonActionBinding = &EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Started, this, &UACWeapon::ZoomToggle);
		//	}
		//	else if (WeaponName == EWeaponName::WeaponName_ShotGun)
		//	{
		//		// Fire Single Shot
		//		LeftMouseButtonActionBinding = &EnhancedInputComponent->BindAction(FireSingleShotAction, ETriggerEvent::Started, this, &UACWeapon::FireMultiProjectile);

		//		// Fire Burst Shot
		//		RightMouseButtonActionBinding = &EnhancedInputComponent->BindAction(FireBurstShotAction, ETriggerEvent::Started, this, &UACWeapon::HandleBurstFire);

		//		// Reload
		//		EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &UACWeapon::HandleReload);
		//	}
		//}
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
	if (CrosshairWidgetClass)
	{
		CrosshairWidget = CreateWidget<UUserWidget>(GetWorld(), CrosshairWidgetClass);
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
		MuzzleFireEffect = WeaponData->FireEffect;
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

	//TODO: ��� �ٲٱ�
	if (WeaponName == EWeaponName::WeaponName_Rifle)
	{
		AttachToComponent(Character->GetArmMesh(), AttachmentRules, FName(TEXT("Gun_Rifle")));
	}
	else if (WeaponName == EWeaponName::WeaponName_ShotGun)
	{
		AttachToComponent(Character->GetArmMesh(), AttachmentRules, FName(TEXT("Gun")));
	}
	else
	{
		AttachToComponent(Character->GetArmMesh(), AttachmentRules, FName(TEXT("Gun")));
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

void UACWeapon::FireSingleProjectile()
{
	if (CurrentState != UnequippedState)
	{
		if (Character == nullptr || Character->GetController() == nullptr)
		{
			return;
		}

		if (HasAmmo())
		{
			//TODO: ����ü Ÿ���� ���������� ���������, �׸��� ������������ ���� Trace ����� �޸� �ؾ���. �켱�� �ܼ� ������ and �Ϲ���(����X)�� ����
			FVector LineTraceStartLocation = Character->GetCamera()->GetComponentLocation();
			FVector LineTraceDirection = Character->GetCamera()->GetForwardVector();
			FVector LineTraceHitLocation;

			if (PerformLineTrace(LineTraceStartLocation, LineTraceDirection, LineTraceMaxDistance, LineTraceHitLocation))
			{
				TargetLocationOfProjectile = LineTraceHitLocation;
			}
			else
			{
				TargetLocationOfProjectile = LineTraceHitLocation;
			}

			// Try and fire a projectile
			if (ProjectileClass != nullptr)
			{
				UWorld* const World = GetWorld();
				if (World != nullptr)
				{
					//// <Old Version>
					//APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
					//const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
					//// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
					//const FVector SpawnLocation = GetOwner()->GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);

					//----------------------------------------------------
					// <New Version>
					const FVector SpawnLocation = this->GetSocketLocation(FName(TEXT("Muzzle")));
					const FRotator SpawnRotation = (TargetLocationOfProjectile - SpawnLocation).Rotation();

					//Set Spawn Collision Handling Override
					FActorSpawnParameters ActorSpawnParams;
					ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

					// Spawn the projectile at the muzzle
					ASuraProjectile* Projectile = World->SpawnActor<ASuraProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
					Projectile->InitializeProjectile(Character);

					SpawnMuzzleFireEffect(SpawnLocation, SpawnRotation);
				}
			}

			// Try and play the sound if specified
			if (FireSound != nullptr)
			{
				UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
			}

			// Try and play a firing animation if specified
			//if (FireAnimation != nullptr)
			//{
			//	// Get the animation object for the arms mesh
			//	//UAnimInstance* AnimInstance = Character->GetArmMesh()->GetAnimInstance();
			//	UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
			//	if (AnimInstance != nullptr)
			//	{
			//		AnimInstance->Montage_Play(FireAnimation, 1.f);
			//	}
			//}
			StartFireAnimation(AM_Fire_Character, AM_Fire_Weapon);

			ConsumeAmmo();

			ApplyCameraShake();

			// <Recoil>
			AddRecoilValue();
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

			// Try and fire a projectile
			if (ProjectileClass != nullptr)
			{
				UWorld* const World = GetWorld();
				if (World != nullptr)
				{
					const FVector SpawnLocation = this->GetSocketLocation(FName(TEXT("Muzzle")));

					for (int pellet = 0; pellet < PelletsNum; pellet++)
					{
						const FRotator SpawnRotation = UKismetMathLibrary::RandomUnitVectorInConeInDegrees((TargetLocationOfProjectile - SpawnLocation).GetSafeNormal(), MaxAngleOfProjectileSpread).Rotation();

						//Set Spawn Collision Handling Override
						FActorSpawnParameters ActorSpawnParams;
						ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

						// Spawn the projectile at the muzzle
						ASuraProjectile* Projectile = World->SpawnActor<ASuraProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
						Projectile->InitializeProjectile(Character);
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

			ApplyCameraShake();

			// <Recoil>
			AddRecoilValue();
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
	if (CurrentState != UnequippedState)
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

bool UACWeapon::PerformLineTrace(FVector StartLocation, FVector LineDirection, float MaxDistance, FVector& HitLocation)
{
	FVector Start = StartLocation;
	FVector End = StartLocation + LineDirection * MaxDistance;

	FHitResult HitResult;

	FCollisionQueryParams Params;
	Params.AddIgnoredComponent(this);
	Params.AddIgnoredActor(Character);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,           // �浹 ��� ����
		Start,               // ���� ����
		End,                 // �� ����
		ECC_Visibility,      // �浹 ä��
		Params               // ���� �Ű�����
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

		DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 1.f, 12, FColor::Red, false, 50.f);
		DrawDebugSphere(GetWorld(), TraceEndedPoint, 1.f, 12, FColor::Blue, false, 50.f);

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
					//// FullAuto Shot
					//// TODO: LeftMouseButtonActionBinding -> Down �� Up���� ������ �����ϱ� 
					//// (�ƴϸ� �׳� Array�� ���������� �����ϰ�, Reset ���� Array ��Ҹ� ���������� ���� �ϴ� ���� ���� ���� ����)
					//LeftMouseButtonActionBinding = &EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &UACWeapon::StartFullAutoShot);
					//EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &UACWeapon::StopFullAutoShot);
					////TODO: Zoom�� Holding ������� �ٲ����
					//// Zoom Toggle
					//RightMouseButtonActionBinding = &EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Started, this, &UACWeapon::ZoomToggle);
					//// Reload
					//RButtonActionBinding = &EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &UACWeapon::HandleReload);

					//------------------------------------------------------------
					InputActionBindingHandles.Add(&EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &UACWeapon::StartFullAutoShot));
					InputActionBindingHandles.Add(&EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &UACWeapon::StopFullAutoShot));
					InputActionBindingHandles.Add(&EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Started, this, &UACWeapon::ZoomToggle));
					InputActionBindingHandles.Add(&EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &UACWeapon::HandleReload));


				}
				else if (WeaponName == EWeaponName::WeaponName_ShotGun)
				{
					//// Fire Single Shot
					////LeftMouseButtonActionBinding = &EnhancedInputComponent->BindAction(FireSingleShotAction, ETriggerEvent::Started, this, &UACWeapon::FireMultiProjectile);
					//LeftMouseButtonActionBinding = &EnhancedInputComponent->BindAction(FireSingleShotAction, ETriggerEvent::Started, this, &UACWeapon::HandleSingleFire);
					//// Fire Burst Shot
					//RightMouseButtonActionBinding = &EnhancedInputComponent->BindAction(FireBurstShotAction, ETriggerEvent::Started, this, &UACWeapon::HandleBurstFire);
					//// Reload
					//RButtonActionBinding = &EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &UACWeapon::HandleReload);

					//-------------------------------------------------------------------
					InputActionBindingHandles.Add(&EnhancedInputComponent->BindAction(FireSingleShotAction, ETriggerEvent::Started, this, &UACWeapon::HandleSingleFire));
					InputActionBindingHandles.Add(&EnhancedInputComponent->BindAction(FireBurstShotAction, ETriggerEvent::Started, this, &UACWeapon::HandleBurstFire));
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
				////EnhancedInputComponent->RemoveBinding(*RightMouseButtonActionBinding);
				////EnhancedInputComponent->RemoveBinding(*LeftMouseButtonActionBinding);
				//if (EnhancedInputComponent->RemoveBinding(*RightMouseButtonActionBinding))
				//{
				//	UE_LOG(LogTemp, Warning, TEXT("Left Action Binding Removed!"));
				//}
				//if (EnhancedInputComponent->RemoveBinding(*LeftMouseButtonActionBinding))
				//{
				//	UE_LOG(LogTemp, Warning, TEXT("Right Action Binding Removed!"));
				//}
				//if (EnhancedInputComponent->RemoveBinding(*RButtonActionBinding))
				//{
				//	UE_LOG(LogTemp, Warning, TEXT("R Action Binding Removed!"));
				//}
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
			//StartReload();
		}
	}
}
void UACWeapon::ReloadingEnd() //Legacy: ��� ����. animation�� ���� ���� ó���� ���� ���� ��.
{
	UE_LOG(LogTemp, Warning, TEXT("Reloading End!!!"));
	//ChangeState(IdleState);
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
	//	if (CrosshairWidgetClass)
	//	{
	//		CrosshairWidget = CreateWidget<UUserWidget>(GetWorld(), CrosshairWidgetClass);
	//		if (CrosshairWidget)
	//		{
	//			CrosshairWidget->AddToViewport();
	//		}
	//	}
	//}
	//else
	//{
	//	if (CrosshairWidget)
	//	{
	//		CrosshairWidget->RemoveFromViewport();
	//		CrosshairWidget = nullptr;
	//	}
	//}

	if (bflag)
	{
		if (CrosshairWidget)
		{
			CrosshairWidget->AddToViewport();
		}
	}
	else
	{
		if (CrosshairWidget)
		{
			CrosshairWidget->RemoveFromViewport();
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
void UACWeapon::StartSingleShot()
{
	if (WeaponName == EWeaponName::WeaponName_Rifle)
	{
		FireSingleProjectile();
	}
	else if (WeaponName == EWeaponName::WeaponName_ShotGun)
	{
		FireMultiProjectile();
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
			GetWorld()->GetTimerManager().SetTimer(FullAutoShotTimer, this, &UACWeapon::FireSingleProjectile, FullAutoShotFireRate, true);
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


#pragma region Recoil
void UACWeapon::AddRecoilValue()
{
	bIsRecoiling = true;

	//float RandRecoilPitch = FMath::RandRange(RecoilAmountPitch * 0.8f, RecoilAmountPitch * 1.2f) * (-1);
	//float RandRecoilYaw = FMath::RandRange(-RecoilAmountYaw, RecoilAmountYaw);

	float RandRecoilPitch = FMath::RandRange(RecoilAmountPitch * RecoilRangeMinPitch , RecoilAmountPitch * RecoilRangeMaxPitch) * (-1);
	float RandRecoilYaw = FMath::RandRange(RecoilAmountYaw * RecoilRangeMinYaw, RecoilAmountYaw * RecoilRangeMaxYaw);

	TotalTargetRecoilValuePitch += RandRecoilPitch;
	TotalTargetRecoilValueYaw += RandRecoilYaw;
}

void UACWeapon::ApplyRecoil(float DeltaTime)
{
	if (Character)
	{
		APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
		if (!PlayerController) return;

		float InterpRecoilTargetValue_Pitch = FMath::FInterpTo(0.f, TotalTargetRecoilValuePitch - CulmulatedRecoilValuePitch, DeltaTime, RecoilSpeed);
		float InterpRecoilTargetValue_Yaw = FMath::FInterpTo(0.f, TotalTargetRecoilValueYaw- CulmulatedRecoilValueYaw, DeltaTime, RecoilSpeed);;

		PlayerController->AddPitchInput(InterpRecoilTargetValue_Pitch);
		PlayerController->AddYawInput(InterpRecoilTargetValue_Yaw);

		CulmulatedRecoilValuePitch += InterpRecoilTargetValue_Pitch;
		CulmulatedRecoilValueYaw += InterpRecoilTargetValue_Yaw;
	}
}

void UACWeapon::RecoverRecoil(float DeltaTime)
{
	APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
	if (!PlayerController) return;

	if (bIsRecoilRecoverAffectedByPlayerInput)
	{
		FVector2D PlayerLookInputVector2D = Character->GetPlayerLookInputVector();

		if (FMath::Sign(PlayerLookInputVector2D.Y) == FMath::Sign((CulmulatedRecoilValuePitch - RecoveredRecoilValuePitch)*(-1)))
		{
			if (PlayerLookInputVector2D.Y >= 0)
			{
				RecoveredRecoilValuePitch += (-1) * FMath::Clamp(PlayerLookInputVector2D.Y, 0.f, (CulmulatedRecoilValuePitch - RecoveredRecoilValuePitch)*(-1));
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

		float InterpRecoilRecoverTargetValue_Pitch = FMath::FInterpTo(0.f, CulmulatedRecoilValuePitch - RecoveredRecoilValuePitch, DeltaTime, RecoilRecoverSpeed);
		float InterpRecoilRecoverTargetValue_Yaw = FMath::FInterpTo(0.f, CulmulatedRecoilValueYaw - RecoveredRecoilValueYaw, DeltaTime, RecoilRecoverSpeed);;

		PlayerController->AddPitchInput(-InterpRecoilRecoverTargetValue_Pitch);
		PlayerController->AddYawInput(-InterpRecoilRecoverTargetValue_Yaw);

		RecoveredRecoilValuePitch += InterpRecoilRecoverTargetValue_Pitch;
		RecoveredRecoilValueYaw += InterpRecoilRecoverTargetValue_Yaw;
	}
	else
	{
		float InterpRecoilRecoverTargetValue_Pitch = FMath::FInterpTo(0.f, CulmulatedRecoilValuePitch - RecoveredRecoilValuePitch, DeltaTime, RecoilRecoverSpeed);
		float InterpRecoilRecoverTargetValue_Yaw = FMath::FInterpTo(0.f, CulmulatedRecoilValueYaw - RecoveredRecoilValueYaw, DeltaTime, RecoilRecoverSpeed);;

		PlayerController->AddPitchInput(-InterpRecoilRecoverTargetValue_Pitch);
		PlayerController->AddYawInput(-InterpRecoilRecoverTargetValue_Yaw);

		RecoveredRecoilValuePitch += InterpRecoilRecoverTargetValue_Pitch;
		RecoveredRecoilValueYaw += InterpRecoilRecoverTargetValue_Yaw;
	}

	if (FMath::Abs(CulmulatedRecoilValuePitch - RecoveredRecoilValuePitch) < KINDA_SMALL_NUMBER
		&& FMath::Abs(CulmulatedRecoilValueYaw - RecoveredRecoilValueYaw) < KINDA_SMALL_NUMBER)
	{
		UE_LOG(LogTemp, Warning, TEXT("Recoil has been perfectly Recovered!!!"));

		// TODO: Reset recoil value �Լ��� ���� ����� ���� ���� �� ��
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
			ApplyRecoil(DeltaTime);
			RecoverRecoil(DeltaTime);
		}
	}
}
#pragma endregion

#pragma region Camera
void UACWeapon::StartCameraSettingChange(FWeaponCamSettingValue* CamSetting)
{
	if (GetWorld()->GetTimerManager().IsTimerActive(CamSettingTimer))
	{
		GetWorld()->GetTimerManager().ClearTimer(CamSettingTimer);
	}

	UpdateCameraSetting(GetWorld()->GetDeltaSeconds(), CamSetting);
}
void UACWeapon::UpdateCameraSetting(float DeltaTime, FWeaponCamSettingValue* CamSetting)
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

			if (CameraRelativeRotation_Error.IsNearlyZero()
				&& FVector::Dist(CamSetting->CameraRelativeLocation, Camera->GetRelativeLocation()) < 0.01
				//&& FMath::Abs(CamSetting->FOV - Camera->FieldOfView) < KINDA_SMALL_NUMBER
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
	UE_LOG(LogTemp, Warning, TEXT("Changing Cam Setting is Completed!!!"));
}
void UACWeapon::ApplyCameraShake()
{
	if (Character)
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
		{
			//PlayerController->ClientStartCameraShake(CameraShake->GetClass());
			//PlayerController->PlayerCameraManager->StartCameraShake(CameraShake->GetClass());

			if (CameraShakeClass)
			{
				PlayerController->PlayerCameraManager->StartCameraShake(CameraShakeClass);
			}
		}
	}
}
#pragma endregion