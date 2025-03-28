// InventoryManager.cpp
#include "ActorComponents/UISystem/ACInventoryManager.h"

#include "AsyncTreeDifferences.h"
#include "SceneTextures.h"
#include "ActorComponents/UISystem/ACUIMangerComponent.h"
#include "ActorComponents/WeaponSystem/ACWeapon.h"
#include "ActorComponents/WeaponSystem/WeaponSystemComponent.h" // 정확한 경로로 바꿔주세요
#include "Components/Image.h"
#include "GameFramework/Character.h"

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

	UpdateCurrentWeaponWindow();

	bWaitingForWeaponSwitch = false;
	PendingWeaponIndex = -1;

	
}

void UACInventoryManager::UpdateCurrentWeaponWindow()
{
	// weaponinventory랑 current index 정보를 써서 attribute창 업데이트! 호출은 inventory widget초기화때랑 current index가 바뀔 때?

	TArray<AWeapon*> WeaponInventory = pWeaponSystemComponent->GetWeaponInventory();
	int32 CurrentWeaponIndex = pWeaponSystemComponent->GetCurrentWeaponIndex();


	// UIManagerComponent 인스턴스 가져오기 (UIManagerComponent가 다른 컴포넌트라면 GetOwner()를 사용하거나 다른 방법으로 접근)
	UACUIMangerComponent* UIManagerComponent = GetOwner()->FindComponentByClass<UACUIMangerComponent>();


	if (UIManagerComponent)
	{
		// GetWidget을 사용하여 이미 생성된 UInventoryWidget 인스턴스를 가져옴
		const UInventoryWidget* InventoryWidget = Cast<UInventoryWidget>(UIManagerComponent->GetWidget(EUIType::Inventory));

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

}







