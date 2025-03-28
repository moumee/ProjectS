// ACInventoryManager.h
#pragma once

#include "CoreMinimal.h"
#include "ACUIMangerComponent.h"
#include "ActorComponents/WeaponSystem/WeaponSystemComponent.h"
#include "Components/ActorComponent.h"
#include "ACInventoryManager.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SURAS_API UACInventoryManager : public UActorComponent
{
	GENERATED_BODY()

public:
	UACInventoryManager();

	virtual void BeginPlay() override;

	//void SetInventoryWidget(UInventoryWidget* InWidget);
	
	void SetPendingWeaponIndex(int32 Index);
	void OnConfirmWeaponEquip();

	//Current Weapon Attribute window update
	void UpdateCurrentWeaponWindow();

protected:

	// Inventory 위젯을 저장할 변수
	// UInventoryWidget* InventoryWidget;

	int32 PendingWeaponIndex = -1; // 버튼을 누르면 바꾸고자 하는 weaponInventory index를 저장하는 변수
	bool bWaitingForWeaponSwitch = false; //

	// WeaponSystemComponent 포인터
	UWeaponSystemComponent* pWeaponSystemComponent;
	
};
