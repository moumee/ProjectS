// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AmmoCounterWidget.generated.h"

class UImage;

UCLASS()

class SURAS_API UAmmoCounterWidget : public UUserWidget
{
	
	GENERATED_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* AmmoCount;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TotalAmmo;

	void UpdateAmmoCount(int32 NewAmmoCount);
	void UpdateTotalAmmo(int32 NewTotalAmmo);

#pragma region HpBar
private:
	// HP 단계별 이미지 배열
	UPROPERTY(meta = (BindWidget))
	UImage* HpBar;

	// 현재 피격 단계 (0~5)
	int32 CurrentHitStage = 0;

	// 마지막으로 피격된 시간
	FTimerHandle RecoveryyTimerHandle;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UTexture2D*> HPBarTextures; // 총 6개 (0~5단계)

public:
	UFUNCTION()
	void UpdateHpBar();

	UFUNCTION()
	void RecoverHpBarStage();
	
	UFUNCTION()
	void ApplyHpBarImage(int32 Stage);
		
#pragma endregion
};
