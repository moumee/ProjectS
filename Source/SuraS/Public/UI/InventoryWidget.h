// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseUIWidget.h"
#include "InventoryWidget.generated.h"


// 위젯 클래스 전방선언
class UTextBlock;
class FReply;
class UWidgetSwitcher;
class UImage;
class UButton;
class UACInventoryManager;

/**
 * 
 */
UENUM(BlueprintType)
enum class EInventoryTab : uint8
{
	Weapon UMETA(DisplayName = "Weapon"),
	Chip UMETA(DisplayName = "Chip")
};


USTRUCT(BlueprintType)
struct FWeaponUI
{
	GENERATED_BODY()

	UPROPERTY()
	UImage* WeaponImage;

	UPROPERTY()
	UTextBlock* WeaponText;

	FWeaponUI() : WeaponImage(nullptr), WeaponText(nullptr) {}
	FWeaponUI(UImage* Image, UTextBlock* Text) : WeaponImage(Image), WeaponText(Text) {};
};

UCLASS()
class SURAS_API UInventoryWidget : public UBaseUIWidget	
{
	GENERATED_BODY()

protected:
	// InventoryManger reference variable
	UACInventoryManager* InventoryManager;
	
	/**  탭관련  **/
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TabWeapon;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TabChip;
	
	EInventoryTab CurrentTab;

	// Widget Switcher
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* ContentSwitcher;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, Category = "Weapon UI")
	UWidgetSwitcher* WeaponWidgetSwitcher;

	/** 사운드 **/
	// 탭 전환 효과음
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundBase* TabSwitchSound;
	
public:
	// 위젯이 생성된 후 호출되는 함수
	virtual void NativeConstruct() override;
	
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	//InventoryManager초기화
	void SetInventoryManager(UACInventoryManager* InVentoryManager);

	// 탭 전환 함수
	void SetActiveTab(EInventoryTab NewTab);

	// 키 입력 함수 (Q, E)
	void SwitchToWeaponTab();
	void SwitchToChipTab();
	

	// 탭 콘텐츠를 보여주는 함수
	void ShowTabContent() const;

	// UI 열고 닫기
	virtual void OpenUI() override;
	
	virtual void CloseUI() override;

#pragma region Weapon
	
	// WBP_Invnetory 초기화
	void InitializeInventory();

	void SwitchPage(int32 PageIndex);
	
	// DTWeapon 포인터 변수
	UDataTable* DTWeapon;
	
	void UpdateWeaponUI(FString WeaponNameStr);

	// // 모든 무기 소유 불값을 false로 만드는 함수
	// void AllWeaponDiscard();

	
	// 총 버튼 클릭시 이벤트 함수
	UFUNCTION() void OnWeaponButtonClicked_Rifle();
	UFUNCTION() void OnWeaponButtonClicked_ShotGun();
	UFUNCTION() void OnWeaponButtonClicked_MissileLauncher();

	// 무기 변경 요청하는 함수
	void RequestChangeWeaponByName(const char* Str);
	
	/** 총기 UI 요소 맵 */
	TMap<FString, FWeaponUI> WeaponUIElements;

	/** 총기 button 바인딩 **/
	UPROPERTY(meta = (BindWidget)) UButton* BtnRifle;
	UPROPERTY(meta = (BindWidget)) UButton* BtnShotGun;
	UPROPERTY(meta = (BindWidget)) UButton* BtnMissileLauncher;
	//UPROPERTY(meta = (BindWidget)) UButton* RailGun;
	

	/** 총기 img 바인딩 **/
	UPROPERTY(meta = (BindWidget)) UImage* Rifle;
	UPROPERTY(meta = (BindWidget)) UImage* ShotGun;
	UPROPERTY(meta = (BindWidget)) UImage* MissileLauncher;
	//UPROPERTY(meta = (BindWidget)) UImage* RailGun;

	/** 총기 이름 바인딩 */
	UPROPERTY(meta = (BindWidget)) UTextBlock* RifleName;
	UPROPERTY(meta = (BindWidget)) UTextBlock* ShotGunName;
	UPROPERTY(meta = (BindWidget)) UTextBlock* MissileLauncherName;
	//UPROPERTY(meta = (BindWidget)) UTextBlock* RailGunName;

	/** 현재 선택 중인 무기 속성창 **/
	UPROPERTY(meta = (BindWidget)) UImage* CurrentWeaponImage;
	//damage
	//speed
	//rebound
	//handle speed
	//magazine
	//firing method
	
	UFUNCTION()
	void OnWeaponPickedUp(FName WeaponName);

private:
	bool bIsInitialized = false;

#pragma endregion Weapon


	
};


