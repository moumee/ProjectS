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

	DTWeapon = GetWeaponDataTable();
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
			WeaponSystem->SwitchToIndex1();
			//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("SwitchToIndex1 호출"));
			break;
		case 1:
			WeaponSystem->SwitchToIndex2();
			//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("SwitchToIndex2 호출"));
			break;
		case 2:
			WeaponSystem->SwitchToIndex3();
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

void UACInventoryManager::UpdateWeaponAttributeUI(AWeapon* Weapon)
{
	if (!Weapon || !Weapon->WeaponData || !Weapon->WeaponData->WeaponImage || !InventoryWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("Weapon 또는 WeaponData 또는 WeaponImage 또는 InventoryWidget이 nullptr입니다."));
		return;
	}

	// 무기 이미지 표시
	InventoryWidget->CurrentWeaponImage->SetBrushFromTexture(Weapon->WeaponData->WeaponImage);
	InventoryWidget->CurrentWeaponImage->SetOpacity(1.0f);

	// Magazine
	if (Weapon->GetAmmoCounterWidget() && InventoryWidget->MagazineTextBlock)
	{
		// UTextBlock* 타입이므로 텍스트를 가져와서 숫자로 변환
		int32 Ammo = FCString::Atoi(*Weapon->GetAmmoCounterWidget()->AmmoCount->GetText().ToString());
		int32 TotalAmmo = FCString::Atoi(*Weapon->GetAmmoCounterWidget()->TotalAmmo->GetText().ToString());

		const FString AmmoString = FString::Printf(TEXT("%d / %d"), Ammo, TotalAmmo);
		InventoryWidget->MagazineTextBlock->SetText(FText::FromString(AmmoString));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AmmoCounterWidget 또는 MagazineTextBlock이 nullptr입니다."));
	}


	
	
	
	// ProjectileData 접근 (CDO + 강제 Load)
	const ASuraProjectile* ProjectileCDO = nullptr;
	if (Weapon->WeaponData->LeftProjectileClass)
	{
		ProjectileCDO = Weapon->WeaponData->LeftProjectileClass->GetDefaultObject<ASuraProjectile>();
		if (ProjectileCDO)
		{
			const_cast<ASuraProjectile*>(ProjectileCDO)->LoadProjectileData();
			const FProjectileData* ProjectileData = ProjectileCDO->GetProjectileData();
			if (ProjectileData)
			{
				const float Damage = ProjectileData->DefaultDamage;
				if (InventoryWidget->CurrentWeaponDamage)
				{
					InventoryWidget->CurrentWeaponDamage->SetPercent(Damage / 100.f);
				}
				if (InventoryWidget->CurrentWeaponDamageText)
				{
					InventoryWidget->CurrentWeaponDamageText->SetText(FText::AsNumber(Damage));
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("ProjectileData가 nullptr입니다."));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Projectile의 CDO 가져오기 실패 (GetDefaultObject)"));
		}
	}

	// FireRate
	const float FireRate = Weapon->WeaponData->FullAutoShotFireRate;
	if (InventoryWidget->CurrentWeaponFireRate)
	{
		InventoryWidget->CurrentWeaponFireRate->SetPercent(FireRate);
	}
	if (InventoryWidget->CurrentWeaponFireRateText)
	{
		InventoryWidget->CurrentWeaponFireRateText->SetText(FText::AsNumber(FireRate * 100.0f));
	}

	// HandleSpeed (Recoil)
	const float Recoil = Weapon->WeaponData->DefaultRecoil.RecoilAmountPitch;
	if (InventoryWidget->CurrentWeaponRecoil)
	{
		InventoryWidget->CurrentWeaponRecoil->SetPercent(Recoil / 10.f);
	}
	if (InventoryWidget->CurrentWeaponRecoilText)
	{
		InventoryWidget->CurrentWeaponRecoilText->SetText(FText::AsNumber(Recoil * 10.f));
	}
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



void UACInventoryManager::UpdateCurrentWeaponWindow()
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
	if (!DTWeapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("DTWeapon is not set in GameInstance!"));
		return;
	}
    
	for (const auto& Row : DTWeapon->GetRowMap())
	{
		FWeaponData* WeaponData = (FWeaponData*)Row.Value;
		if (WeaponData)
		{
			WeaponData->bIsWeaponOwned = false;
			
			InventoryWidget->UpdateWeaponUI(Row.Key.ToString());
		}
	}
    
	UE_LOG(LogTemp, Warning, TEXT("All weapons have been discarded."));
}

void UACInventoryManager::ChangeWeaponByName(const FString& WeaponNameStr)
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
        if (!Weapon || !Weapon->WeaponData)
        {
            GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Yellow,
                FString::Printf(TEXT("[%d] 무기 또는 WeaponData가 nullptr"), i));
            continue;
        }

        FString WeaponEnumStr = UEnum::GetDisplayValueAsText(Weapon->WeaponData->WeaponName).ToString();

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

void UACInventoryManager::UnlockWeapon(FName WeaponName)
{
	if (!DTWeapon) return;

	// WeaponName_ 접두어를 제거하고, 실제 이름만 추출
	FString WeaponNameStr = WeaponName.ToString().RightChop(24);  // "EWeaponName::WeaponName_"을 제거

	// 수정된 WeaponNameStr을 사용하여 FindRow 호출
	static const FString ContextString(TEXT("Weapon Unlock Context"));
	FWeaponData* WeaponData = DTWeapon->FindRow<FWeaponData>(*WeaponNameStr, ContextString);

	if (WeaponData && !WeaponData->bIsWeaponOwned)
	{
		WeaponData->bIsWeaponOwned = true;
		InventoryWidget->UpdateWeaponUI(WeaponNameStr);
	}
}

void UACInventoryManager::CreateAndAddWeaponFromData(FWeaponData* WeaponData)
{
	if (!WeaponData || !WeaponData->bIsWeaponOwned) return;

	UWorld* World = GetWorld();
	if (!World) return;

	// 📌 1. SuraWeaponPickUp 임시 생성
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ASuraWeaponPickUp* TempPickUp = World->SpawnActor<ASuraWeaponPickUp>(ASuraWeaponPickUp::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	if (!TempPickUp) return;

	// 📌 2. 무기 정보 입력
	TempPickUp->SetWeaponClass(WeaponData->WeaponClass); // Setter 만들어야 함
	TempPickUp->SetWeaponName(WeaponData->WeaponName);   // Setter 만들어야 함

	// 📌 3. 무기 생성
	AWeapon* NewWeapon = TempPickUp->SpawnWeapon(Cast<ASuraPawnPlayer>(GetOwner()));
	TempPickUp->Destroy(); // PickUp 액터 제거

	if (!NewWeapon) return;

	// 📌 4. 무기 인벤토리에 추가
	pWeaponSystemComponent->GetWeaponInventory().Add(NewWeapon);
}


void UACInventoryManager::InitializeOwnedWeaponsFromDT()
{
	if (!DTWeapon) return;

	const TMap<FName, uint8*>& RowMap = DTWeapon->GetRowMap();

	for (const auto& Pair : RowMap)
	{
		FWeaponData* WeaponData = reinterpret_cast<FWeaponData*>(Pair.Value);
		if (WeaponData && WeaponData->bIsWeaponOwned)
		{
			CreateAndAddWeaponFromData(WeaponData);
		}
	}

	// 자동 장착
	if (pWeaponSystemComponent && pWeaponSystemComponent->GetWeaponInventory().Num() > 0)
	{
		pWeaponSystemComponent->EquipFirstWeapon();
	}
}








