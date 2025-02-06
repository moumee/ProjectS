
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

#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Animation/AnimInstance.h"
#include "Engine/LocalPlayer.h" //TODO: ������ ���� header���� �˾ƺ�����
#include "Engine/World.h"

#include "Camera/CameraComponent.h"
//#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"

// Sets default values for this component's properties
UACWeapon::UACWeapon()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true; //TODO: Tick Ȱ���ϸ� Ȱ��ȭ�ؾ���.

	// ...

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
	//SetCollisionObjectType(ECC_GameTraceChannel3); //Weapon
	//SetCollisionResponseToAllChannels(ECR_Ignore);
}

void UACWeapon::InitializeWeapon(ASuraCharacterPlayerWeapon* NewCharacter)
{
	Character = NewCharacter;

	// Set up action bindings
	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		//TODO: �Ʒ� �ڵ忡 ���ؼ� �˾ƺ�����. Multi Play�� ���� ���ΰ�?
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
			Subsystem->AddMappingContext(FireMappingContext, 1);
		}

		//TODO: Weapon Type�� ���� Bind�Ǵ� Action�� �ٸ����� �����ϱ�
		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			if (WeaponName == EWeaponName::WeaponName_Rifle)
			{
				// FullAuto Shot
				EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &UACWeapon::StartFullAutoShot);
				EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Completed, this, &UACWeapon::StopFullAutoShot);

				//TODO: Zoom�� Holding ������� �ٲ����
				// Zoom Toggle
				RightMouseButtonActionBinding = &EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Started, this, &UACWeapon::ZoomToggle);
			}
			else if (WeaponName == EWeaponName::WeaponName_ShotGun)
			{
				// Fire Single Shot
				LeftMouseButtonActionBinding = &EnhancedInputComponent->BindAction(FireSingleShotAction, ETriggerEvent::Started, this, &UACWeapon::FireMultiProjectile);

				// Fire Burst Shot
				EnhancedInputComponent->BindAction(FireBurstShotAction, ETriggerEvent::Started, this, &UACWeapon::HandleBurstFire);

				// Reload
				EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Started, this, &UACWeapon::HandleReload);
			}
		}
	}

	// Set Aim Socket Relative Transform
	SetAimSocketRelativeTransform();
}

// Called when the game starts
void UACWeapon::BeginPlay()
{
	Super::BeginPlay();

	// ...

	//TODO: WeaponType�� ���� �����Ǵ� State ��ü���� �޶������� �����ϱ�
	IdleState = NewObject<USuraWeaponIdleState>(this, USuraWeaponIdleState::StaticClass());
	FiringState = NewObject<USuraWeaponFiringState>(this, USuraWeaponFiringState::StaticClass());
	UnequippedState = NewObject<USuraWeaponUnequippedState>(this, USuraWeaponUnequippedState::StaticClass());
	ReloadingState = NewObject<USuraWeaponReloadingState>(this, USuraWeaponReloadingState::StaticClass());

	ChangeState(UnequippedState);


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

	//TODO: ���� ���� Ȯ�� ��� �����ϱ�. �ټ� ���⸦ ������ ���� �ֱ� ������,
	// �׸��� WeaponSystemComponent���� weapon ���� ������ �ٷ� ���̱� ������ ���� �����ؾ���
	// �׸��� �÷��̾ �ѹ� ȹ���� ����� WeaponSystem�� WeaponInventory�� ����Ǿ� ���������� ��� �����ϵ��� �ؾ���
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
	// AttachToComponent(Character->GetArmMesh(), AttachmentRules, FName(TEXT("GripPoint")));
	AttachToComponent(Character->GetMesh(), AttachmentRules, FName(TEXT("Gun")));

	//TODO: �� ���� ����� ������ ������
	//RightHandToAimSocketOffset = this->GetSocketLocation(FName(TEXT("Aim"))) - Character->GetMesh()->GetSocketLocation(FName("Gun"));
	RightHandToAimSocketOffset = this->GetSocketLocation(FName(TEXT("Aim"))) - Character->GetMesh()->GetBoneLocation(FName(TEXT("hand_r")));
	if (Character->GetWeaponSystem() && Character->GetWeaponSystem()->GetClass()->ImplementsInterface(UWeaponInterface::StaticClass()))
	{
		Character->GetWeaponSystem()->SetRightHandToAimSocketOffset(RightHandToAimSocketOffset);
	}

	// Set AimSocketTransform
	SetAimSocketTransform();

	// Set Up Widget UI Class
	ActivateCrosshairWidget(true);

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
		if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				//Subsystem->RemoveMappingContext(FireMappingContext);
			}

			if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
			{
				//EnhancedInputComponent->RemoveBinding(*LeftMouseButtonActionBinding);
				//EnhancedInputComponent->RemoveBinding(*RightMouseButtonActionBinding);
			}
		}

		DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		Character = nullptr;

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

				// <New Version>
				const FVector SpawnLocation = this->GetSocketLocation(FName(TEXT("Muzzle")));
				const FRotator SpawnRotation = (TargetLocationOfProjectile - SpawnLocation).Rotation();

				//Set Spawn Collision Handling Override
				FActorSpawnParameters ActorSpawnParams;
				ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

				// Spawn the projectile at the muzzle
				ASuraProjectile* Projectile = World->SpawnActor<ASuraProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
				Projectile->InitializeProjectile(Character);
			}
		}

		// Try and play the sound if specified
		if (FireSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
		}

		// Try and play a firing animation if specified
		if (FireAnimation != nullptr)
		{
			// Get the animation object for the arms mesh
			//UAnimInstance* AnimInstance = Character->GetArmMesh()->GetAnimInstance();
			UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
			if (AnimInstance != nullptr)
			{
				AnimInstance->Montage_Play(FireAnimation, 1.f);
			}
		}

		// <Recoil>
		AddRecoilValue();
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
			}
		}

		// Try and play the sound if specified
		if (FireSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, Character->GetActorLocation());
		}

		// Try and play a firing animation if specified
		if (FireAnimation != nullptr)
		{
			// Get the animation object for the arms mesh
			//UAnimInstance* AnimInstance = Character->GetArmMesh()->GetAnimInstance();
			UAnimInstance* AnimInstance = Character->GetMesh()->GetAnimInstance();
			if (AnimInstance != nullptr)
			{
				AnimInstance->Montage_Play(FireAnimation, 1.f);
			}
		}

		// <Recoil>
		AddRecoilValue();
	}
}


void UACWeapon::ZoomToggle()
{
	if (CurrentState != UnequippedState)
	{
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

	if (Character->GetWeaponSystem() && Character->GetWeaponSystem()->GetClass()->ImplementsInterface(UWeaponInterface::StaticClass()))
	{
		Character->GetWeaponSystem()->ZoomIn(true);
	}
	ActivateCrosshairWidget(false);
}

void UACWeapon::ZoomOut()
{
	bIsZoomIn = false;

	if (Character->GetWeaponSystem() && Character->GetWeaponSystem()->GetClass()->ImplementsInterface(UWeaponInterface::StaticClass()))
	{
		Character->GetWeaponSystem()->ZoomIn(false);
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
	FTransform IKHandGunTransform = Character->GetMesh()->GetSocketTransform(FName(TEXT("ik_hand_gun")));

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
void UACWeapon::EquipWeapon(ASuraCharacterPlayerWeapon* TargetCharacter)
{
	// TODO: �Լ� ����ȭ �ؾ���
	UE_LOG(LogTemp, Warning, TEXT("Equip Weapon!!!"));
	AttachWeaponToPlayer(TargetCharacter);

	ChangeState(IdleState);
}

void UACWeapon::UnequipWeapon(ASuraCharacterPlayerWeapon* TargetCharacter)
{
	// TODO: Detach ���� ó���ؾ��ϴ� �͵� ó���ؾ���
	UE_LOG(LogTemp, Warning, TEXT("Unequip Weapon!!!"));
	DetachWeaponFromPlayer();

	ChangeState(UnequippedState);
}
#pragma endregion

#pragma region Reload
void UACWeapon::HandleReload()
{
	if (CurrentState == IdleState)
	{
		ChangeState(ReloadingState);
	}
}
#pragma endregion

void UACWeapon::ActivateCrosshairWidget(bool bflag)
{
	if (bflag)
	{
		if (CrosshairWidgetClass)
		{
			CrosshairWidget = CreateWidget<UUserWidget>(GetWorld(), CrosshairWidgetClass);
			if (CrosshairWidget)
			{
				CrosshairWidget->AddToViewport();
			}
		}
	}
	else
	{
		if (CrosshairWidget)
		{
			CrosshairWidget->RemoveFromViewport();
			CrosshairWidget = nullptr;
		}
	}
}

#pragma region FireMode
void UACWeapon::HandleSingleFire()
{
}

void UACWeapon::HandleBurstFire()
{
	if (CurrentState != UnequippedState
		&& CurrentState != FiringState)
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
	UE_LOG(LogTemp, Warning, TEXT("FullAutoShot Started!!!"));
	if (!GetWorld()->GetTimerManager().IsTimerActive(FullAutoShotTimer))
	{
		FireSingleProjectile();
		GetWorld()->GetTimerManager().SetTimer(FullAutoShotTimer, this, &UACWeapon::FireSingleProjectile, FullAutoShotFireRate, true);
	}
}

void UACWeapon::StopFullAutoShot()
{
	UE_LOG(LogTemp, Warning, TEXT("FullAutoShot Ended!!!"));
	GetWorld()->GetTimerManager().ClearTimer(FullAutoShotTimer);
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