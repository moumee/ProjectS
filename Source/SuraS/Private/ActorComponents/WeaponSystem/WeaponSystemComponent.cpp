


#include "ActorComponents/WeaponSystem/WeaponSystemComponent.h"

//#include "ActorComponents/WeaponSystem/SuraCharacterPlayerWeapon.h" //TODO: Player Class �����ϱ�
#include "Characters/PawnBasePlayer/SuraPawnPlayer.h"

#include "ActorComponents/WeaponSystem/SuraWeaponPickUp.h"
#include "ActorComponents/WeaponSystem/ACWeapon.h"
#include "ActorComponents/WeaponSystem/WeaponName.h"
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

#include "UI/CustomGameInstance.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Blueprint/UserWidget.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

UWeaponSystemComponent::UWeaponSystemComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UWeaponSystemComponent::BeginPlay()
{
	Super::BeginPlay();
	InitializePlayerReference();
	LoadWSCData();
	InitStartingWeapons();
}

void UWeaponSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	SearchWeapon(); //TODO: 굳이 Tick으로 할 이유가 없을 듯. 필요시에 트리거로 작동하도록 해야함
	//CalculateScreenCenterWorldPositionAndDirection(ScreenCenterWorldLocation, ScreenCenterWorldDirection);
	CalculateTargetRightHandPosition();
}

#pragma region PlayerReference
void UWeaponSystemComponent::InitializePlayerReference()
{
	ASuraPawnPlayer* NewPlayerOwner = Cast<ASuraPawnPlayer>(GetOwner());
	if (IsValid(NewPlayerOwner))
	{
		PlayerOwner = NewPlayerOwner;
		PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		//PlayerController = Cast<APlayerController>(PlayerOwner->GetController())

		// Set up action bindings
		if (PlayerController)
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
			{
				// Set the priority of the mapping to 1, so that it overrides the Jump action with the Fire action when using touch input
				Subsystem->AddMappingContext(WeaponSystemMappingContext, 1);
			}

			if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
			{
				// Inereact With WeaponPickUp
				EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &UWeaponSystemComponent::PickUpWeapon);
	
				// Switch Weapon
				EnhancedInputComponent->BindAction(SwitchWeaponUpAction, ETriggerEvent::Started, this, &UWeaponSystemComponent::SwitchToNextWeapon);
				EnhancedInputComponent->BindAction(SwitchWeaponDownAction, ETriggerEvent::Started, this, &UWeaponSystemComponent::SwitchToPreviousWeapon);

				EnhancedInputComponent->BindActionValueLambda(SwitchWeapon1Action, ETriggerEvent::Started, [this](const FInputActionValue& InputActionValue, int32 idx) {SwitchToIndex(idx);}, 0);
				EnhancedInputComponent->BindActionValueLambda(SwitchWeapon2Action, ETriggerEvent::Started, [this](const FInputActionValue& InputActionValue, int32 idx) {SwitchToIndex(idx);}, 1);
				EnhancedInputComponent->BindActionValueLambda(SwitchWeapon3Action, ETriggerEvent::Started, [this](const FInputActionValue& InputActionValue, int32 idx) {SwitchToIndex(idx);}, 2);
			}
		}
	}
}
bool UWeaponSystemComponent::IsSceneCaptureActive()
{
	return bUseSceneCapture;
}
void UWeaponSystemComponent::LoadWSCData()
{
	DTWSC = WSCDataTableHandle.GetRow<FWeaponSystemComponentData>("");
	if (DTWSC)
	{
		bUseSceneCapture = DTWSC->bUseSceneCapture;
	}

	if (bUseSceneCapture)
	{
		if (!PlayerOwner) { return; }
		USceneCaptureComponent2D* FPSceneCapture = PlayerOwner->GetSceneCaptureComponent();
		if (!FPSceneCapture) { return; }
		FPSceneCapture->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;
		FPSceneCapture->ShowOnlyComponent(PlayerOwner->GetArmMesh());
		FPSceneCapture->ShowOnlyComponent(PlayerOwner->GetHandsMesh());

		PlayerOwner->GetArmMesh()->SetVisibleInSceneCaptureOnly(true);
		PlayerOwner->GetHandsMesh()->SetVisibleInSceneCaptureOnly(true);

		if (FPHUD) { FPHUD->AddToViewport(); }
	}
}
void UWeaponSystemComponent::InitStartingWeapons()
{
	if (!DTWSC) return;
	const TMap<EWeaponName, bool> WeaponOwnerShipMap = DTWSC->WeaponOwnerShipMap;
	const TMap<EWeaponName, TSubclassOf<AWeapon>> WeaponClasses = DTWSC->WeaponClasses;

	UCustomGameInstance* GameInstance = Cast<UCustomGameInstance>(GetWorld()->GetGameInstance());

	for (auto& Elem : WeaponOwnerShipMap)
	{
		bool bDoesGameInstanceHasWeapon = false;
		if (GameInstance)
		{
			if (GameInstance->OwnedWeapons.Contains(Elem.Key))
			{
				if (GameInstance->OwnedWeapons[Elem.Key])
				{
					bDoesGameInstanceHasWeapon = true;
				}
			}
		}

		if (Elem.Value || bDoesGameInstanceHasWeapon)
		{
			UWorld* World = GetWorld();
			if (!World) return;
			if (!WeaponClasses.Find(Elem.Key)) continue;
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			AWeapon* NewWeapon = World->SpawnActor<AWeapon>(WeaponClasses.Find(Elem.Key)->Get(), FTransform(), ActorSpawnParams);
			if (!NewWeapon) continue;
			NewWeapon->InitializeWeapon(Cast<ASuraPawnPlayer>(GetOwner()));

			AddNewWeaponToInventory(NewWeapon);

			if (!GameInstance) return;
			if (GameInstance->OwnedWeapons.Contains(Elem.Key))
			{
				GameInstance->OwnedWeapons[Elem.Key] = true;
			}
			else
			{
				GameInstance->OwnedWeapons.Emplace(Elem.Key, true);
			}
		}
	}

	for (int32 i = 0; i < WeaponInventory.Num(); i++)
	{
		if (WeaponInventory[i]->GetWeaponName() == DTWSC->StartingWeaponName)
		{
			//CurrentWeaponIndex = i;
			//ChangeWeapon(CurrentWeaponIndex);
			//CurrentWeaponIndex = 0;
			//SwitchToIndex(i);

			int32 PrevIdx = CurrentWeaponIndex;
			CurrentWeaponIndex = i;
			CurrentWeapon = WeaponInventory[i];
			CurrentWeapon->SwitchWeapon(PlayerOwner, true);
			OnWeaponSwitched.Broadcast(PrevIdx, CurrentWeaponIndex);

			return;
		}
	}

	AWeapon* NewWeapon;

	if (DTWSC->WeaponClasses.Contains(DTWSC->StartingWeaponName))
	{
		UWorld* const World = GetWorld();
		if (World != nullptr && PlayerOwner != nullptr)
		{
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			NewWeapon = GetWorld()->SpawnActor<AWeapon>(DTWSC->WeaponClasses[DTWSC->StartingWeaponName], PlayerOwner->GetActorTransform(), ActorSpawnParams);
			NewWeapon->InitializeWeapon(PlayerOwner);
		}
	}

	WeaponInventory.AddUnique(NewWeapon);

	if (CurrentWeapon == nullptr)
	{
		//CurrentWeapon = NewWeapon;
		//CurrentWeapon->SwitchWeapon(PlayerOwner, true);

		int32 PrevIdx = CurrentWeaponIndex;
		CurrentWeaponIndex = WeaponInventory.Num() - 1;
		CurrentWeapon = NewWeapon;
		CurrentWeapon->SwitchWeapon(PlayerOwner, true);
		OnWeaponSwitched.Broadcast(PrevIdx, CurrentWeaponIndex);
	}
}
#pragma endregion

#pragma region SearchWeapon
bool UWeaponSystemComponent::SearchWeapon()
{
	TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes;
	traceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Visibility)); //TODO: Customize Collision Channel

	TArray<AActor*> ignoreActors;
	ignoreActors.Init(PlayerOwner, 1);

	FVector sphereSpwanLocation = PlayerOwner->GetActorLocation();

	TArray<AActor*> overlappedActors;

	bool bIsWeaponInViewPort = UKismetSystemLibrary::SphereOverlapActors(GetWorld(), sphereSpwanLocation, SearchWeaponRadius, traceObjectTypes, nullptr, ignoreActors, overlappedActors);

	float MinDistanceToWeapon = SearchWeaponRadius;
	ASuraWeaponPickUp* NearestWeapon = nullptr;

	for (AActor* overlappedActor : overlappedActors)
	{
		ASuraWeaponPickUp* WeaponObject = Cast<ASuraWeaponPickUp>(overlappedActor);
		if (WeaponObject != nullptr)
		{
			if (IsInViewport(GetScreenPositionOfWorldLocation(WeaponObject->GetActorLocation()).Get<0>(), SearchWeaponViewportRatio_Width, SearchWeaponViewportRatio_Height))
			{
				float DistanceToWeapon = PlayerOwner->GetDistanceTo(WeaponObject);
				if (DistanceToWeapon < MinDistanceToWeapon)
				{
					MinDistanceToWeapon = DistanceToWeapon;
					NearestWeapon = WeaponObject;

					// TODO: Overlapped weapon UI on/off
				}
			}
		}
	}

	OverlappedWeapon = NearestWeapon;

	return bIsWeaponInViewPort;
}

TTuple<FVector2D, bool> UWeaponSystemComponent::GetScreenPositionOfWorldLocation(const FVector& SearchLocation) const
{
	FVector2D ScreenLocation = FVector2D::ZeroVector;
	bool bResult = UGameplayStatics::ProjectWorldToScreen(PlayerController, SearchLocation, ScreenLocation);

	return MakeTuple(ScreenLocation, bResult);
}

bool UWeaponSystemComponent::IsInViewport(FVector2D ActorScreenPosition, float ScreenRatio_Width, float ScreenRatio_Height) const
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

#pragma region Interaction

void UWeaponSystemComponent::PickUpWeapon()
{
	if (OverlappedWeapon != nullptr)
	{
		if (CurrentWeapon == nullptr || (CurrentWeapon != nullptr && CurrentWeapon->GetCurrentState()->GetWeaponStateType() == EWeaponStateType::WeaponStateType_Idle))
		{
			if (OverlappedWeapon->IsMagazine())
			{
				ObtainAmmo(OverlappedWeapon);
			}
			else
			{
				ObtainNewWeapon(OverlappedWeapon);
			}
		}
	}
}
bool UWeaponSystemComponent::ObtainNewWeapon(ASuraWeaponPickUp* NewWeaponPickUp)
{
	if (!NewWeaponPickUp || !PlayerOwner)
	{
		return false;
	}
	const EWeaponName NewWeaponName = NewWeaponPickUp->GetWeaponName();
	const int32 NewAmmo = NewWeaponPickUp->GetAmmo();

	auto TryAddAmmoIfAlreadyOwned = [&](TArray<AWeapon*>& Inventory) -> bool
	{
		for (AWeapon* Weapon : Inventory)
		{
			if (Weapon && Weapon->GetWeaponName() == NewWeaponName)
			{
				UE_LOG(LogTemp, Warning, TEXT("Already Possess"));
				if (Weapon->AddAmmo(NewAmmo))
				{
					NewWeaponPickUp->DestroyWeaponPickUp();
				}
				return true;
			}
		}
		return false;
	};

	if (TryAddAmmoIfAlreadyOwned(WeaponInventory) || TryAddAmmoIfAlreadyOwned(SkillWeaponInventory))
	{
		return false;
	}

	AWeapon* NewWeapon = NewWeaponPickUp->SpawnWeapon(PlayerOwner);
	if (!NewWeapon) { return false; }
	NewWeaponPickUp->DestroyWeaponPickUp();

	if (NewWeapon->IsSkillWeapon())
	{
		SkillWeaponInventory.AddUnique(NewWeapon);
		CurrentSkillWeapon = NewWeapon;
		CurrentSkillWeapon->EquipWeapon(PlayerOwner, true);

		//suhyeon 여기에 skillweapon획득 델리게이트 연결
		// 스킬 무기 장착 후 델리게이트를 브로드캐스트
		OnSkillWeaponEquipped.Broadcast(CurrentSkillWeapon);
	}
	else
	{
		WeaponInventory.AddUnique(NewWeapon);
		if (!CurrentWeapon)
		{
			CurrentWeapon = NewWeapon;
			CurrentWeapon->SwitchWeapon(PlayerOwner, true);
		}
	}

	//FName WeaponNameAsFName = FName(*UEnum::GetValueAsString(NewWeaponPickUp->GetWeaponName()));
	OnWeaponPickedUp.Broadcast(NewWeaponPickUp->GetWeaponName()); /** suhyeon **/

	return true;
}

bool UWeaponSystemComponent::ObtainAmmo(ASuraWeaponPickUp* MagazinePickUp)
{
	for (AWeapon* WeaponInPossession : WeaponInventory)
	{
		if (WeaponInPossession->GetWeaponName() == MagazinePickUp->GetWeaponName())
		{
			if (WeaponInPossession->AddAmmo(MagazinePickUp->GetAmmo()))
			{
				MagazinePickUp->DestroyWeaponPickUp();
				return true;
			}
		}
	}

	for (AWeapon* WeaponInPossession : SkillWeaponInventory)
	{
		if (WeaponInPossession->GetWeaponName() == MagazinePickUp->GetWeaponName())
		{
			if (WeaponInPossession->AddAmmo(MagazinePickUp->GetAmmo()))
			{
				MagazinePickUp->DestroyWeaponPickUp();
				return true;
			}
		}
	}

	return false;
}

#pragma endregion

void UWeaponSystemComponent::ZoomIn(bool bZoomIn)
{
	bIsZoomIn = bZoomIn;

	//UE_LOG(LogTemp, Warning, TEXT("ZOOM Toggled!!!"));
}

bool UWeaponSystemComponent::IsWeaponModifyingCamFov()
{
	if (CurrentWeapon)
	{
		if (CurrentWeapon->IsModifyingPlayerCamFov() || bIsZoomIn)
		{
			return true;
		}
	}
	return false;
}

#pragma endregion

#pragma region Aiming

void UWeaponSystemComponent::SetRightHandToAimSocketOffset(FVector offset)
{
	RightHandToAimSocketOffset = offset;
}

FVector UWeaponSystemComponent::CalculateScreenCenterWorldPositionAndDirection(FVector& OutWorldPosition, FVector& OutWorldDirection) const
{
	//APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController || !PlayerController->PlayerCameraManager)
	{
		return FVector::ZeroVector;
	}

	// ȭ�� ũ�� ��������
	FVector2D ViewportSize = GEngine->GameViewport->Viewport->GetSizeXY();

	// ȭ�� �߾� ��ǥ ���?
	FVector2D ScreenCenter(ViewportSize.X / 2.0f, ViewportSize.Y / 2.0f);

	// ȭ�� �߽��� ���� ��ġ�� ���� ��������
	FVector WorldPosition, WorldDirection;
	if (PlayerController->DeprojectScreenPositionToWorld(ScreenCenter.X, ScreenCenter.Y, WorldPosition, WorldDirection))
	{
		OutWorldPosition = WorldPosition;
		OutWorldDirection = WorldDirection;
		return WorldPosition + (WorldDirection * 15.0f); // TODO: ���� �Ÿ��� �������? ��ġ ���� �����ϵ��� �����ϱ�
	}

	return FVector::ZeroVector;
}

FVector UWeaponSystemComponent::CalculateTargetRightHandPosition()
{
	const FVector AimLocation = CalculateScreenCenterWorldPositionAndDirection(ScreenCenterWorldLocation, ScreenCenterWorldDirection);

	const FRotator MeshRotation = PlayerOwner->GetArmMesh()->GetComponentRotation();

	const FVector TargetRightHandLocation = AimLocation - MeshRotation.RotateVector(RightHandToAimSocketOffset);

	TargetRightHandWorldLocation = TargetRightHandLocation;

	return TargetRightHandLocation;
}

FTransform UWeaponSystemComponent::GetWeaponAimSocketRelativeTransform()
{
	return FTransform();
}
#pragma endregion


#pragma region SwitchWeapon
bool UWeaponSystemComponent::IsCurrentSkillWeaponTargeting()
{
	if (CurrentWeapon && CurrentWeapon->GetCurrentState()->GetWeaponStateType() == EWeaponStateType::WeaponStateType_Waiting) { return true; }
	else { return false; }
}
void UWeaponSystemComponent::SwitchToPreviousWeapon()
{
	if (IsCurrentSkillWeaponTargeting()) { return; }
	if (WeaponInventory.Num() > 1)
	{
		const int32 PrevIndex = CurrentWeaponIndex;
		
		CurrentWeaponIndex--;
		if (CurrentWeaponIndex < 0)
		{
			CurrentWeaponIndex = WeaponInventory.Num() + CurrentWeaponIndex;
		}
		ChangeWeapon(CurrentWeaponIndex);

		UE_LOG(LogTemp, Warning, TEXT("Broadcasting weapon switch: %d -> %d"), PrevIndex, CurrentWeaponIndex);
		OnWeaponSwitched.Broadcast(PrevIndex, CurrentWeaponIndex);
	}
}
void UWeaponSystemComponent::SwitchToNextWeapon()
{
	if (IsCurrentSkillWeaponTargeting()) { return; }
	if (WeaponInventory.Num() > 1)
	{
		const int32 PrevIndex = CurrentWeaponIndex;
		
		CurrentWeaponIndex = (CurrentWeaponIndex + 1) % WeaponInventory.Num();
		ChangeWeapon(CurrentWeaponIndex);

		UE_LOG(LogTemp, Warning, TEXT("Broadcasting weapon switch: %d -> %d"), PrevIndex, CurrentWeaponIndex);
		OnWeaponSwitched.Broadcast(PrevIndex, CurrentWeaponIndex);
	}
}
void UWeaponSystemComponent::SwitchToIndex(int32 idx)
{
	if (IsCurrentSkillWeaponTargeting()) { return; }
	if (WeaponInventory.IsValidIndex(idx) && CurrentWeaponIndex != idx)
	{
		const int32 PrevIndex = CurrentWeaponIndex;
		CurrentWeaponIndex = idx;
		ChangeWeapon(CurrentWeaponIndex);

		UE_LOG(LogTemp, Warning, TEXT("Broadcasting weapon switch: %d -> %d"), PrevIndex, CurrentWeaponIndex);
		OnWeaponSwitched.Broadcast(PrevIndex, CurrentWeaponIndex);
	}
}
void UWeaponSystemComponent::SwitchToOtherWeapon()
{
	WeaponInventory[CurrentWeaponIndex]->SwitchWeapon(PlayerOwner, true);
	CurrentWeapon = WeaponInventory[CurrentWeaponIndex];
}
void UWeaponSystemComponent::ChangeWeapon(int32 WeaponIndex)
{
	if (WeaponInventory.IsValidIndex(WeaponIndex))
	{
		if (IsValid(CurrentWeapon))
		{
			CurrentWeapon->SwitchWeapon(PlayerOwner, false);
		}
	}
}
#pragma endregion

void UWeaponSystemComponent::AddNewWeaponToInventory(AWeapon* NewWeapon)
{
	if (!NewWeapon) { return; }
	if (NewWeapon->IsSkillWeapon())
	{
		SkillWeaponInventory.AddUnique(NewWeapon);
		if (!CurrentSkillWeapon)
		{
			CurrentSkillWeaponIndex = SkillWeaponInventory.Num() - 1;
			CurrentSkillWeapon = NewWeapon;
			CurrentSkillWeapon->EquipWeapon(PlayerOwner, true);
		}
		// 스킬 무기 장착 후 skillweapon획득 델리게이트를 브로드캐스트
		OnSkillWeaponEquipped.Broadcast(CurrentSkillWeapon);
	}
	else
	{
		WeaponInventory.AddUnique(NewWeapon);
		//if (!CurrentWeapon)
		//{
		//	int32 PrevIdx = CurrentWeaponIndex;
		//	CurrentWeaponIndex = WeaponInventory.Num() - 1;
		//	CurrentWeapon = NewWeapon;
		//	CurrentWeapon->SwitchWeapon(PlayerOwner, true);
		//	OnWeaponSwitched.Broadcast(PrevIdx, CurrentWeaponIndex);
		//}
	}
}

#pragma region Control
bool UWeaponSystemComponent::TryTakeControl(AWeapon* NewWeapon)
{
	if (!CurrentWeapon)
	{
		ControllingWeapon = NewWeapon;
		return true;
	}
	else
	{
		EWeaponStateType CurrWeaponStateType = CurrentWeapon->GetCurrentState()->GetWeaponStateType();
		if (CurrWeaponStateType == EWeaponStateType::WeaponStateType_Idle) //TODO: 다른 state일 때 스킬 어떻게 작동해야 하는지
		{
			CurrentWeapon->ChangeState(CurrentWeapon->WaitingState);
			ControllingWeapon = NewWeapon;
			return true;
		}
		else
		{
			return false;
		}
	}
}
void UWeaponSystemComponent::ReleaseControl()
{
	if (!CurrentWeapon)
	{
		ControllingWeapon = nullptr;
	}
	else
	{
		EWeaponStateType CurrWeaponStateType = CurrentWeapon->GetCurrentState()->GetWeaponStateType();
		if (CurrWeaponStateType == EWeaponStateType::WeaponStateType_Waiting)
		{
			CurrentWeapon->ChangeState(CurrentWeapon->IdleState);
			ControllingWeapon = CurrentWeapon;
		}
		else
		{
		}
	}
}
#pragma endregion
