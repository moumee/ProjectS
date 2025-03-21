// InventoryManager.cpp
#include "ActorComponents/UISystem/ACInventoryManager.h"

#include "AsyncTreeDifferences.h"
#include "SceneTextures.h"
#include "ActorComponents/UISystem/ACUIMangerComponent.h"
#include "ActorComponents/WeaponSystem/WeaponSystemComponent.h" // 정확한 경로로 바꿔주세요
#include "GameFramework/Character.h"

UACInventoryManager::UACInventoryManager()
{
	
}

// void UACInventoryManager::SetInventoryWidget(UInventoryWidget* InWidget)
// {
// 	InventoryWidget = InWidget; // InventoryWidget 참조.	UImanagerComponent에서 참조하는 걸 그대로 인자로 넘김.
// }

void UACInventoryManager::SetPendingWeaponIndex(const int32 Index)
{
	PendingWeaponIndex = Index;
	bWaitingForWeaponSwitch = true;

	// if (GEngine)
	// {
	// 	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("F 키를 누르면 무기가 교체 됩니다."));
	// }
}

void UACInventoryManager::OnConfirmWeaponEquip()
{
	// if (!bWaitingForWeaponSwitch || PendingWeaponIndex < 0) return;
	//
	// ACharacter* OwnerCharacter = Cast<ACharacter>(GetOuter());
	// if (!OwnerCharacter) return;
	//
	// UWeaponSystemComponent* WeaponSystem = OwnerCharacter->FindComponentByClass<UWeaponSystemComponent>();
	// if (!WeaponSystem) return;
	//
	// WeaponSystem->ChangeWeapon(PendingWeaponIndex); // WeaponSystemComponent의 ChangeWeapon 함수 호출
	// GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("ChangeWeapon함수 호출됨"));
	//
	// bWaitingForWeaponSwitch = false;
	// PendingWeaponIndex = -1;

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
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan,
		FString::Printf(TEXT("PendingWeaponIndex: %d"), PendingWeaponIndex));

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

	bWaitingForWeaponSwitch = false;
	PendingWeaponIndex = -1;

	
}







