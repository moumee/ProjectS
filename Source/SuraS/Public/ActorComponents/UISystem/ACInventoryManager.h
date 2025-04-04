// ACInventoryManager.h
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ACInventoryManager.generated.h"

class UInventoryWidget;
class UWeaponSystemComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SURAS_API UACInventoryManager : public UActorComponent
{
	GENERATED_BODY()

public:
	UACInventoryManager();

	virtual void BeginPlay() override;

	//inventorywidget 참조
	void SetInventoryWidget(UInventoryWidget* InWidget);
	UInventoryWidget* GetInventoryWidget() const;

	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	// UDataTable* DTWeapon;  // 블루프린트에서 데이터 테이블 할당
	
	void SetPendingWeaponIndex(int32 Index);
	void OnConfirmWeaponEquip();

	//Current Weapon Attribute window update
	void UpdateClickedWeaponWindow();
	void UpdateCurrentWeaponWindow();

	// 모든 무기 소유 불값을 false로 만드는 함수
	void AllWeaponDiscard();

	// 무기 변경 요청하는 함수
	void ChangeWeaponByName(const FString& WeaponNameStr);

	// 무기 해금
	void UnlockWeapon(FName WeaponName);


private:

	//Inventory 위젯을 저장할 변수
	UPROPERTY()
	UInventoryWidget* InventoryWidget;

	int32 PendingWeaponIndex = -1; // 버튼을 누르면 바꾸고자 하는 weaponInventory index를 저장하는 변수
	bool bWaitingForWeaponSwitch = false; //

	// WeaponSystemComponent 포인터
	UWeaponSystemComponent* pWeaponSystemComponent;

	 // 블루프린트에서 데이터 테이블 할당
	
};
