// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseUIWidget.h"
#include "ActorComponents/WeaponSystem/WeaponName.h"
#include "PlayerHUD.generated.h"

/**
 * 
 */

class UHitScreenWidget;
class UWeaponSystemComponent;
class UPlayerHitWidget;
class UImage;
class ASuraPawnPlayer;
class UOverlay;
class AWeapon;
class UWidgetAnimation;
class UTextBlock;
class UACPlayerHUDManager;

UCLASS()
class SURAS_API UPlayerHUD : public UBaseUIWidget
{
	GENERATED_BODY()

	UPROPERTY()
	UACPlayerHUDManager* PlayerHUDManager = nullptr;


#pragma region HpBar
private:
	// HP 단계별 이미지 배열
	UPROPERTY(meta = (BindWidget))
	UImage* HpBar = nullptr;

	// 현재 피격 단계 (0~5)
	int32 CurrentHitStage = 0;
	
	// // 마지막으로 피격된 시간
	// FTimerHandle RecoveryTimerHandle;

	UPROPERTY()
	UHitScreenWidget* PlayerHitWidget = nullptr;

	UPROPERTY()
	ASuraPawnPlayer* SuraPawnPlayer = nullptr;
	

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* CurrentHitStageText = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UTexture2D*> HPBarTextures; // 총 6개 (0~5단계)
	

public:
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);
	void NativeConstruct();

	void SetPlayerHUDManager(UACPlayerHUDManager* InManager);
	
	UFUNCTION()
	void ApplyHpBarImage(float healthRatio);
	

#pragma endregion
	
#pragma region WeaponCarousel
private:
	UPROPERTY(meta = (BindWidget))
	UOverlay* WeaponSlot_0 = nullptr;
	UPROPERTY(meta = (BindWidget))
	UOverlay* WeaponSlot_1 = nullptr;
	// UPROPERTY(meta = (BindWidget))
	// UOverlay* WeaponSlot_2;

	UPROPERTY(meta = (BindWidget))
	UImage* Image_0 = nullptr;
	UPROPERTY(meta = (BindWidget))
	UImage* Image_1 = nullptr;
	// UPROPERTY(meta = (BindWidget))
	// UImage* Image_2;

	// UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim, AllowPrivateAccess = "true"))
	// UWidgetAnimation* Slot0_CenterToBottom;
	// UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim, AllowPrivateAccess = "true"))
	// UWidgetAnimation* Slot0_BottomToTop;
	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim, AllowPrivateAccess = "true"))
	UWidgetAnimation* Slot0_TopToCenter = nullptr;
	// UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim, AllowPrivateAccess = "true"))
	// UWidgetAnimation* Slot1_CenterToBottom;
	// UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim, AllowPrivateAccess = "true"))
	// UWidgetAnimation* Slot1_BottomToTop;
	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim, AllowPrivateAccess = "true"))
	UWidgetAnimation* Slot1_TopToCenter = nullptr;
	// UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim, AllowPrivateAccess = "true"))
	// UWidgetAnimation* Slot2_CenterToBottom;
	// UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim, AllowPrivateAccess = "true"))
	// UWidgetAnimation* Slot2_BottomToTop;
	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim, AllowPrivateAccess = "true"))
	UWidgetAnimation* Slot2_TopToCenter = nullptr;

	UPROPERTY(meta=(BindWidget))
	UTextBlock* WeaponInventoryNum = nullptr;

	UPROPERTY(meta=(BindWidget))
	UImage* background = nullptr;

	int32 CurrentWeaponIndex = 0; // 인벤토리 기준 현재 무기 인덱스

	UPROPERTY()
	UWeaponSystemComponent* WeaponSystemComponent = nullptr;

	// Animation 키 -> 애니메이션 포인터 매핑
	UPROPERTY()
	TMap<FString, UWidgetAnimation*> AnimationMap;



public:
	// weaponinventory에서 인덱스에 맞게 weaponslot에 img를 할당해주는 함수
	void UpdateWeaponIcons();
	// weaponinventory에서 currentweapon에 해당하는 인덱스와 나머지 총기들의 인덱스 번호를 참조하여 weaponslot의 상 중 하 위치를 지정하는 함수
	// weaponslot의 위치를 바꾸는 함수.
	// 애니메이션 함수.
	UFUNCTION()
	void OnWeaponSwitchAnim(int32 PrevIndex, int32 NewIndex);

	UFUNCTION()
	void OnHealthUpdated(float NewHealth, float OldHealth, float MaxHealth, AActor* DamageCauser);
	UFUNCTION()
	void InitializeHUD() const;
	
	UFUNCTION()
	void UpdatePickup(EWeaponName WeaponName); // <JaeHyeong> FName -> EWeaponName 수정

	int32 GetWeaponCount() const;
	void HandleWeaponSlotUIUpdate(int32 PrevIndex, int32 NewIndex);
	void UpdateWeaponSlotUIByInventoryOrder();


#pragma endregion
};

