// InventoryManager.cpp
#include "ActorComponents/UISystem/ACInventoryManager.h"

#include "ActorComponents/UISystem/ACUIMangerComponent.h"
#include "ActorComponents/WeaponSystem/ACWeapon.h"
#include "ActorComponents/WeaponSystem/WeaponSystemComponent.h" 
#include "Components/Image.h"
#include "GameFramework/Character.h"
#include "UI/InventoryWidget.h"

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
}

void UACInventoryManager::SetInventoryWidget(UInventoryWidget* InWidget)
{
	InventoryWidget = InWidget;
}

UInventoryWidget* UACInventoryManager::GetInventoryWidget() const
{
	return InventoryWidget;
}

void UACInventoryManager::SetUIManager(UACUIMangerComponent* UIMangerComponent)
{
	UIManager = UIMangerComponent;
}

UDataTable* UACInventoryManager::GetWeaponDataTable() const
{
	return UIManager ? UIManager->GetWeaponDataTable() : nullptr;
}


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

	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()); // ← 여기! GetOuter() 대신 GetOwner() 써야 안전
	if (!OwnerCharacter)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("OwnerCharacter null"));
		return;
	}

	UWeaponSystemComponent* WeaponSystem = OwnerCharacter->FindComponentByClass<UWeaponSystemComponent>();
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

void UACInventoryManager::UpdateClickedWeaponWindow()
{
	// weaponinventory랑 current index 정보를 써서 attribute창 업데이트! 호출은 inventory widget초기화때랑 current index가 바뀔 때?

	TArray<AWeapon*> WeaponInventory = pWeaponSystemComponent->GetWeaponInventory();
	
	if (InventoryWidget && WeaponInventory.IsValidIndex(PendingWeaponIndex))
	{
		AWeapon* ClickedWeapon = WeaponInventory[PendingWeaponIndex];
		if (ClickedWeapon && ClickedWeapon->WeaponData && ClickedWeapon->WeaponData->WeaponImage)
		{
			InventoryWidget->CurrentWeaponImage->SetBrushFromTexture(ClickedWeapon->WeaponData->WeaponImage);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Weapon 또는 WeaponData 또는 WeaponImage가 nullptr입니다."));
		}
	}
}



void UACInventoryManager::UpdateCurrentWeaponWindow()
{
	// weaponinventory랑 current index 정보를 써서 attribute창 업데이트! 호출은 inventory widget초기화때랑 current index가 바뀔 때?

	TArray<AWeapon*> WeaponInventory = pWeaponSystemComponent->GetWeaponInventory();
	int32 CurrentWeaponIndex = pWeaponSystemComponent->GetCurrentWeaponIndex();


	if (InventoryWidget && WeaponInventory.IsValidIndex(CurrentWeaponIndex))
	{
		AWeapon* CurrentWeapon = WeaponInventory[CurrentWeaponIndex];
		if (CurrentWeapon && CurrentWeapon->WeaponData && CurrentWeapon->WeaponData->WeaponImage)
		{
			InventoryWidget->CurrentWeaponImage->SetBrushFromTexture(CurrentWeapon->WeaponData->WeaponImage);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Weapon 또는 WeaponData 또는 WeaponImage가 nullptr입니다."));
		}
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







