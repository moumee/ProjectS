// InventoryManager.cpp
#include "ActorComponents/UISystem/ACInventoryManager.h"

#include "ActorComponents/UISystem/ACUIMangerComponent.h"
#include "ActorComponents/WeaponSystem/ACWeapon.h"
#include "ActorComponents/WeaponSystem/SuraProjectile.h"
#include "ActorComponents/WeaponSystem/SuraWeaponPickUp.h"
#include "ActorComponents/WeaponSystem/WeaponSystemComponent.h" 
#include "Characters/PawnBasePlayer/SuraPawnPlayer.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "UI/InventoryWidget.h"
#include "UI/CustomGameInstance.h"
#include "Components/TextBlock.h"
#include "ActorComponents/WeaponSystem/AmmoCounterWidget.h"

UACInventoryManager::UACInventoryManager()
{
}

void UACInventoryManager::BeginPlay()
{
	Super::BeginPlay();
	
	if (!pWeaponSystemComponent)
	{
		pWeaponSystemComponent = GetOwner()->FindComponentByClass<UWeaponSystemComponent>();
		if (!pWeaponSystemComponent)
		{
			UE_LOG(LogTemp, Error, TEXT("UWeaponSystemComponent를 찾을 수 없습니다."));
		}
	}

	GameInstance = Cast<UCustomGameInstance>(GetWorld()->GetGameInstance()); // <JaeHyeong>
	DTWeapon = GetWeaponDataTable();
	DTWSC = GetWSCDataTable();  // <JaeHyeong>
	// DTProjectile = GetProjectileDataTable();

	InitializeOwnedWeaponsFromDT(); // dt_weapon에서 소유한 무기들을 weapon inventory에 동기화
}

void UACInventoryManager::SetInventoryWidget(UInventoryWidget* InWidget)
{
	InventoryWidget = InWidget;
}

void UACInventoryManager::SetUIManager(UACUIMangerComponent* UIMangerComponent)
{
	UIManager = UIMangerComponent;
}

UDataTable* UACInventoryManager::GetWeaponDataTable() const
{
	return UIManager ? UIManager->GetWeaponDataTable() : nullptr;
}

UDataTable* UACInventoryManager::GetWSCDataTable() const // <JaeHyeong>
{
	return UIManager ? UIManager->GetWSCDataTable() : nullptr;
}

// UDataTable* UACInventoryManager::GetProjectileDataTable() const
// {
// 	return UIManager ? UIManager->GetProjectileDataTable() : nullptr;
// }


void UACInventoryManager::SetPendingWeaponIndex(const int32 Index)
{

	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan,
	FString::Printf(TEXT("PendingWeaponIndex: %d"), PendingWeaponIndex));
	
	PendingWeaponIndex = Index;
	bWaitingForWeaponSwitch = true;

	UpdateClickedWeaponWindow();
}

void UACInventoryManager::OnConfirmWeaponEquip()
{
	UE_LOG(LogTemp, Warning, TEXT("✅ OnConfirmWeaponEquip 호출됨"));
	
	if (!bWaitingForWeaponSwitch)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("bWaitingForWeaponSwitch == false"));
		return;
	}

	if (PendingWeaponIndex < 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("PendingWeaponIndex < 0"));
		return;
	}

	ASuraPawnPlayer* OwnerPawn = Cast<ASuraPawnPlayer>(GetOwner());
	if (!OwnerPawn)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("OwnerPawn null"));
		return;
	}

	UWeaponSystemComponent* WeaponSystem = OwnerPawn->FindComponentByClass<UWeaponSystemComponent>();
	if (!WeaponSystem)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("WeaponSystemComponent 못 찾음"));
		return;
	}

	const TArray<AWeapon*>& Inventory = WeaponSystem->GetWeaponInventory();
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green,
		FString::Printf(TEXT("무기 인벤토리 개수: %d"), Inventory.Num()));


	if (!Inventory.IsValidIndex(PendingWeaponIndex))
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("유효하지 않은 인덱스입니다."));
		return;
	}

	//WeaponSystem->ChangeWeapon(PendingWeaponIndex);  changeWeapon은 currentindex를 바꿔주는 코드가 없어서 무기가 안 바뀜
	// WeaponSystem->SwitchToIndex3(); change weapon 대신에 switchtoindex를 호출하면 되긴해서 조건문으로 설정하면 될듯
	switch (PendingWeaponIndex)
	{
		case 0:
			WeaponSystem->SwitchToIndex(0);
			//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("SwitchToIndex1 호출"));
			break;
		case 1:
			WeaponSystem->SwitchToIndex(1);
			//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("SwitchToIndex2 호출"));
			break;
		case 2:
			WeaponSystem->SwitchToIndex(2);
			//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("SwitchToIndex3 호출"));
			break;
		default:
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Invalid index for weapon switching."));
			break;
	}

	//UpdateCurrentWeaponWindow();

	bWaitingForWeaponSwitch = false;
	PendingWeaponIndex = -1;
}

void UACInventoryManager::UpdateWeaponAttributeUI(AWeapon* Weapon) // <JaeHyeong> 전체적으로 수정
{
	if (!Weapon || !InventoryWidget) return;
	UDataTable* Table = Weapon->WeaponDataTable ? Weapon->WeaponDataTable.LoadSynchronous() : nullptr;
	if (!Table) return;
	const FWeaponData* Row = Table->FindRow<FWeaponData>(Weapon->WeaponRowName, TEXT("UpdateWeaponAttributeUI"));
	if (!Row) return;

	// <Image>
	if (InventoryWidget->CurrentWeaponImage)
	{
		if (Row->WeaponImage_Inventory)
		{
			InventoryWidget->CurrentWeaponImage->SetBrushFromTexture(Row->WeaponImage_Inventory);
			InventoryWidget->CurrentWeaponImage->SetOpacity(1.0f);
		}
	}

	// <Ammo>
	if (InventoryWidget->MagazineTextBlock)
	{
		const int32 Ammo = Weapon->GetLeftAmmoInCurrentMag();
		const int32 TotalAmmo = Weapon->GetMaxAmmoPerMag();
		InventoryWidget->MagazineTextBlock->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), Ammo, TotalAmmo)));
	}

	// Projectile
	if (Row->LeftProjectileClass)
	{
		const ASuraProjectile* ProjectileCDO = Row->LeftProjectileClass->GetDefaultObject<ASuraProjectile>();
		if (ProjectileCDO)
		{
			const FProjectileData* ProjectileData = ProjectileCDO->GetProjectileData();
			if (ProjectileData)
			{
				const float Damage = ProjectileData->DefaultDamage;
				if (InventoryWidget->CurrentWeaponDamage)
				{
					const float DamagePercent = FMath::Clamp(Damage / 100.f, 0.f, 1.f);
					InventoryWidget->CurrentWeaponDamage->SetPercent(DamagePercent);
				}
				if (InventoryWidget->CurrentWeaponDamageText)
				{
					InventoryWidget->CurrentWeaponDamageText->SetText(FText::AsNumber(Damage));
				}
			}
		}
	}

	// 5) FireRate
	{
		const float FireRate = Row->FullAutoShotFireRate;
		if (InventoryWidget->CurrentWeaponFireRate)
		{
			const float FireRatePercent = FMath::Clamp(FireRate / 20.f, 0.f, 1.f);
			InventoryWidget->CurrentWeaponFireRate->SetPercent(FireRatePercent);
		}
		if (InventoryWidget->CurrentWeaponFireRateText)
		{
			InventoryWidget->CurrentWeaponFireRateText->SetText(FText::AsNumber(FireRate));
		}
	}

	// 6) Recoil
	{
		const float RecoilPitch = Row->DefaultRecoil.RecoilAmountPitch;
		if (InventoryWidget->CurrentWeaponRecoil)
		{
			const float RecoilPercent = FMath::Clamp(RecoilPitch / 10.f, 0.f, 1.f);
			InventoryWidget->CurrentWeaponRecoil->SetPercent(RecoilPercent);
		}
		if (InventoryWidget->CurrentWeaponRecoilText)
		{
			InventoryWidget->CurrentWeaponRecoilText->SetText(FText::AsNumber(RecoilPitch));
		}
	}


	////----------------------

	//if (!Weapon || !Weapon->WeaponDataTable || !Weapon->WeaponDataTable.LoadSynchronous()->FindRow<FWeaponData>(Weapon->WeaponRowName, TEXT("LoadWeaponData"))->WeaponImage_Inventory || !InventoryWidget)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Weapon 또는 WeaponData 또는 WeaponImage 또는 InventoryWidget이 nullptr입니다."));
	//	return;
	//}

	//// 무기 이미지 표시
	////InventoryWidget->CurrentWeaponImage->SetBrushFromTexture(Weapon->WeaponDataTableHandle.GetRow<FWeaponData>("")->WeaponImage);
	//InventoryWidget->CurrentWeaponImage->SetBrushFromTexture(Weapon->WeaponDataTable.LoadSynchronous()->FindRow<FWeaponData>(Weapon->WeaponRowName, TEXT("LoadWeaponData"))->WeaponImage_Inventory);
	//InventoryWidget->CurrentWeaponImage->SetOpacity(1.0f);

	//// Magazine
	//if (Weapon->GetAmmoCounterWidget() && InventoryWidget->MagazineTextBlock)
	//{
	//	// UTextBlock* 타입이므로 텍스트를 가져와서 숫자로 변환
	//	int32 Ammo = FCString::Atoi(*Weapon->GetAmmoCounterWidget()->AmmoCount->GetText().ToString());
	//	int32 TotalAmmo = FCString::Atoi(*Weapon->GetAmmoCounterWidget()->TotalAmmo->GetText().ToString());

	//	const FString AmmoString = FString::Printf(TEXT("%d / %d"), Ammo, TotalAmmo);
	//	InventoryWidget->MagazineTextBlock->SetText(FText::FromString(AmmoString));
	//}
	//else
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("AmmoCounterWidget 또는 MagazineTextBlock이 nullptr입니다."));
	//}


	//// ProjectileData 접근 (CDO + 강제 Load)
	//const ASuraProjectile* ProjectileCDO = nullptr;
	//if (Weapon->WeaponDataTable.LoadSynchronous()->FindRow<FWeaponData>(Weapon->WeaponRowName, TEXT("LoadWeaponData"))->LeftProjectileClass)
	//{
	//	ProjectileCDO = Weapon->WeaponDataTable.LoadSynchronous()->FindRow<FWeaponData>(Weapon->WeaponRowName, TEXT("LoadWeaponData"))->LeftProjectileClass->GetDefaultObject<ASuraProjectile>();
	//	if (ProjectileCDO)
	//	{
	//		const_cast<ASuraProjectile*>(ProjectileCDO)->LoadProjectileData();
	//		const FProjectileData* ProjectileData = ProjectileCDO->GetProjectileData();
	//		if (ProjectileData)
	//		{
	//			const float Damage = ProjectileData->DefaultDamage;
	//			if (InventoryWidget->CurrentWeaponDamage)
	//			{
	//				InventoryWidget->CurrentWeaponDamage->SetPercent(Damage / 100.f);
	//			}
	//			if (InventoryWidget->CurrentWeaponDamageText)
	//			{
	//				InventoryWidget->CurrentWeaponDamageText->SetText(FText::AsNumber(Damage));
	//			}
	//		}
	//		else
	//		{
	//			UE_LOG(LogTemp, Error, TEXT("ProjectileData가 nullptr입니다."));
	//		}
	//	}
	//	else
	//	{
	//		UE_LOG(LogTemp, Error, TEXT("Projectile의 CDO 가져오기 실패 (GetDefaultObject)"));
	//	}
	//}

	//// FireRate
	//const float FireRate = Weapon->WeaponDataTable.LoadSynchronous()->FindRow<FWeaponData>(Weapon->WeaponRowName, TEXT("LoadWeaponData"))->FullAutoShotFireRate;
	//if (InventoryWidget->CurrentWeaponFireRate)
	//{
	//	InventoryWidget->CurrentWeaponFireRate->SetPercent(FireRate);
	//}
	//if (InventoryWidget->CurrentWeaponFireRateText)
	//{
	//	InventoryWidget->CurrentWeaponFireRateText->SetText(FText::AsNumber(FireRate * 100.0f));
	//}

	//// HandleSpeed (Recoil)
	//const float Recoil = Weapon->WeaponDataTable.LoadSynchronous()->FindRow<FWeaponData>(Weapon->WeaponRowName, TEXT("LoadWeaponData"))->DefaultRecoil.RecoilAmountPitch;
	//if (InventoryWidget->CurrentWeaponRecoil)
	//{
	//	InventoryWidget->CurrentWeaponRecoil->SetPercent(Recoil / 10.f);
	//}
	//if (InventoryWidget->CurrentWeaponRecoilText)
	//{
	//	InventoryWidget->CurrentWeaponRecoilText->SetText(FText::AsNumber(Recoil * 10.f));
	//}
}

void UACInventoryManager::UpdateClickedWeaponWindow()
{
	// weaponinventory랑 current index 정보를 써서 attribute창 업데이트! 호출은 inventory widget초기화때랑 current index가 바뀔 때?

	TArray<AWeapon*> WeaponInventory = pWeaponSystemComponent->GetWeaponInventory();

	if (WeaponInventory.IsValidIndex(PendingWeaponIndex))
	{
		AWeapon* ClickedWeapon = WeaponInventory[PendingWeaponIndex];
		UpdateWeaponAttributeUI(ClickedWeapon);
	}
}

void UACInventoryManager::UpdateCurrentWeaponWindow() //TODO: 이 부분 한번 살펴봐야 할 듯
{
	// weaponinventory랑 current index 정보를 써서 attribute창 업데이트! 호출은 inventory widget초기화때랑 current index가 바뀔 때?

	TArray<AWeapon*> WeaponInventory = pWeaponSystemComponent->GetWeaponInventory();
	int32 CurrentWeaponIndex = pWeaponSystemComponent->GetCurrentWeaponIndex();

	if (WeaponInventory.IsValidIndex(CurrentWeaponIndex))
	{
		AWeapon* CurrentWeapon = WeaponInventory[CurrentWeaponIndex];
		UpdateWeaponAttributeUI(CurrentWeapon);
	}
}

void UACInventoryManager::AllWeaponDiscard()
{
	//// <Old Version>
	//if (!DTWeapon)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("DTWeapon is not set in GameInstance!"));
	//	return;
	//}
 //   
	//for (const auto& Row : DTWeapon->GetRowMap())
	//{
	//	FWeaponData* WeaponData = (FWeaponData*)Row.Value;
	//	if (WeaponData)
	//	{
	//		WeaponData->bIsWeaponOwned = false;
	//		
	//		InventoryWidget->UpdateWeaponUI(Row.Key.ToString());
	//	}
	//}
 //   
	//UE_LOG(LogTemp, Warning, TEXT("All weapons have been discarded."));

	//----------------------------------------------
	// <New Version>
	if (!GameInstance) return;
	for (auto& Elem : GameInstance->OwnedWeapons)
	{
		//Elem.Value = false;
		//FString WeaponNameStr = FName(*UEnum::GetValueAsString(Elem.Key)).ToString().RightChop(24);  // "EWeaponName::WeaponName_"을 제거
		//InventoryWidget->UpdateWeaponUI(WeaponNameStr);

		Elem.Value = false;
		InventoryWidget->UpdateWeaponUI(Elem.Key);
	}
}

void UACInventoryManager::ChangeWeaponByName(const FString& WeaponNameStr) //TODO: 살펴봐야함
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Cyan,
        FString::Printf(TEXT("요청된 무기 이름: %s"), *WeaponNameStr));

    if (!pWeaponSystemComponent)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("WeaponSystemComponent 찾기 실패"));
        return;
    }

    const TArray<AWeapon*>& Inventory = pWeaponSystemComponent->GetWeaponInventory();
    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green,
        FString::Printf(TEXT("WeaponInventory에 있는 무기 수: %d"), Inventory.Num()));

    // 무기 인벤토리 탐색
    for (int32 i = 0; i < Inventory.Num(); ++i)
    {
        AWeapon* Weapon = Inventory[i];
        if (!Weapon || !Weapon->WeaponDataTable.LoadSynchronous()->FindRow<FWeaponData>(Weapon->WeaponRowName, TEXT("LoadWeaponData")))
        {
            GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow,
                FString::Printf(TEXT("[%d] 무기 또는 WeaponData가 nullptr"), i));
            continue;
        }

        FString WeaponEnumStr = UEnum::GetDisplayValueAsText(Weapon->WeaponDataTable.LoadSynchronous()->FindRow<FWeaponData>(Weapon->WeaponRowName, TEXT("LoadWeaponData"))->WeaponName).ToString();

        // GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::White,
        //     FString::Printf(TEXT("[%d] 무기 이름: %s"), i, *WeaponEnumStr));

        if (WeaponEnumStr == WeaponNameStr)
        {
            GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green,
                FString::Printf(TEXT("일치하는 무기 인덱스: %d"), i));

            GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("무기를 교체하려면 F키를 눌러주세요")));

            SetPendingWeaponIndex(i);
            return;
        }
    }

    GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red,
        FString::Printf(TEXT("'%s' 무기를 WeaponInventory에서 찾을 수 없음"), *WeaponNameStr));
}

void UACInventoryManager::UnlockWeapon(EWeaponName WeaponName)
{
	//// <Old Version>
	//if (!DTWeapon) return;

	//// WeaponName_ 접두어를 제거하고, 실제 이름만 추출
	//FString WeaponNameStr = WeaponName.ToString().RightChop(24);  // "EWeaponName::WeaponName_"을 제거

	//// 수정된 WeaponNameStr을 사용하여 FindRow 호출
	//static const FString ContextString(TEXT("Weapon Unlock Context"));
	//FWeaponData* WeaponData = DTWeapon->FindRow<FWeaponData>(*WeaponNameStr, ContextString);

	//if (WeaponData && !WeaponData->bIsWeaponOwned)
	//{
	//	WeaponData->bIsWeaponOwned = true;
	//	InventoryWidget->UpdateWeaponUI(WeaponNameStr);
	//}
	// 
	//-------------------------------------------------------------------
	// <New Version>
	if (!GameInstance) return;
	if (GameInstance->OwnedWeapons.Contains(WeaponName))
	{
		GameInstance->OwnedWeapons[WeaponName] = true;
		InventoryWidget->UpdateWeaponUI(WeaponName);
	}
	else
	{
		GameInstance->OwnedWeapons.Add(WeaponName, true);
		InventoryWidget->UpdateWeaponUI(WeaponName);
	}
}

//void UACInventoryManager::CreateAndAddWeaponFromData(FWeaponData* WeaponData) //TODO: TempPickUp을 Spawn할 이유가 없음. WSC에서 
//{
//	if (!WeaponData || !WeaponData->bIsWeaponOwned) return; //TODO: DT_WSC 정보로 변경
//
//	UWorld* World = GetWorld();
//	if (!World) return;
//
//	// <JaeHyeong> 
//	//----------------------
//	// 0) WeaponClass soft ref -> 실제 UClass 동기 로드
//	TSubclassOf<AWeapon> ResolvedWeaponClass = nullptr;
//
//	if (WeaponData->WeaponClass.IsNull())
//	{
//		UE_LOG(LogTemp, Warning, TEXT("WeaponClass is null in row for %s"), *UEnum::GetValueAsString(WeaponData->WeaponName));
//		return;
//	}
//	else if (UClass* AlreadyLoaded = WeaponData->WeaponClass.Get()) //TODO: DT_WSC에 각 WeaponName 별 클래스 정보 받아오기
//	{
//		ResolvedWeaponClass = AlreadyLoaded; // 이미 메모리에 있으면 그대로 사용
//	}
//	else
//	{
//		// 디스크에서 바로 로드(블로킹). 히치가 우려되면 아래 B안 사용.
//		ResolvedWeaponClass = WeaponData->WeaponClass.LoadSynchronous();
//		if (!ResolvedWeaponClass)
//		{
//			UE_LOG(LogTemp, Error, TEXT("Failed to LoadSynchronous WeaponClass for %s"), *UEnum::GetValueAsString(WeaponData->WeaponName));
//			return;
//		}
//	}
//	//-----------------
//
//	// 📌 1. SuraWeaponPickUp 임시 생성
//	FActorSpawnParameters SpawnParams;
//	SpawnParams.Owner = GetOwner();
//	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
//
//	ASuraWeaponPickUp* TempPickUp = World->SpawnActor<ASuraWeaponPickUp>(ASuraWeaponPickUp::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
//	if (!TempPickUp) return;
//
//	// 📌 2. 무기 정보 입력
//	TempPickUp->SetWeaponClass(ResolvedWeaponClass); // Setter 만들어야 함
//	TempPickUp->SetWeaponName(WeaponData->WeaponName);   // Setter 만들어야 함
//
//	// 📌 3. 무기 생성
//	AWeapon* NewWeapon = TempPickUp->SpawnWeapon(Cast<ASuraPawnPlayer>(GetOwner()));
//	TempPickUp->Destroy(); // PickUp 액터 제거
//
//	if (!NewWeapon) return;
//
//	// 📌 4. 무기 인벤토리에 추가
//	pWeaponSystemComponent->GetWeaponInventory().Add(NewWeapon);
//}

void UACInventoryManager::InitializeOwnedWeaponsFromDT()
{
	//// <Old Version>
	//if (!DTWeapon) return;

	//const TMap<FName, uint8*>& RowMap = DTWeapon->GetRowMap();

	//for (const auto& Pair : RowMap)
	//{
	//	FWeaponData* WeaponData = reinterpret_cast<FWeaponData*>(Pair.Value);
	//	if (WeaponData && WeaponData->bIsWeaponOwned) //TODO: DT_Weapon 말고 DT_WSC에서 무기 소유정보 받아오도록 수정하기
	//	{
	//		CreateAndAddWeaponFromData(WeaponData);
	//	}
	//}

	//// 자동 장착
	//if (pWeaponSystemComponent && pWeaponSystemComponent->GetWeaponInventory().Num() > 0)
	//{
	//	pWeaponSystemComponent->EquipFirstWeapon();
	//}

	//-----------------------------------------------------
	// <New Version>
	if (!DTWSC) return;
	const TMap<FName, uint8*>& RowMap = DTWSC->GetRowMap();
	for (const auto& Pair : RowMap)
	{
		FWeaponSystemComponentData* WSCData = reinterpret_cast<FWeaponSystemComponentData*>(Pair.Value);
		if (!WSCData) return;
		const TMap<EWeaponName, bool> WeaponOwnerShipMap = WSCData->WeaponOwnerShipMap;
		const TMap<EWeaponName, TSubclassOf<AWeapon>> WeaponClasses = WSCData->WeaponClasses;
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
				if (!pWeaponSystemComponent) return;
				pWeaponSystemComponent->AddNewWeaponToInventory(NewWeapon);


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
	}
}








