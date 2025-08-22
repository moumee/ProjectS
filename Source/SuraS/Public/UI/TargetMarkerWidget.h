// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TargetMarkerWidget.generated.h"


/**
 * 
 */

UENUM(BlueprintType)
enum class ETargetLockOnState : uint8
{
	None = 0 UMETA(DisplayName = "None"),
	TargetingReady = 1 UMETA(DisplayName = "TargetingReady"),
	TargetingSuccess = 2 UMETA(DisplayName = "TargetingSuccess")
};

UCLASS()
class SURAS_API UTargetMarkerWidget : public UUserWidget
{
	GENERATED_BODY()


protected:
	UPROPERTY(meta = (BindWidget))
	class UImage* MarkerImage;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* BoolTest;

	ETargetLockOnState CurrentLockOnState = ETargetLockOnState::None;
	
	float CurrentTimerDelay = 0.f;

	float TargetingReadyDuration = 0.f;
	float TargetingSuccessDuration = 0.f;
	UPROPERTY(Transient)
	float AnimationElapsedTime = 0.0f; // 애니메이션 경과 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	float AnimationDuration; // 애니메이션 총 지속 시간

public:
	bool bIsLockedOn = false; // 락온 완료 상태를 나타내는 플래그

	void StartLockOnProcess(float InReadyDuration, float InSuccessDruation); // 0.3초 타이머를 시작하는 함수
	void UpdateLockOnTimer(float RealTimeDeltaSeconds); // 매 프레임 타이머를 업데이트하는 함수

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
};
