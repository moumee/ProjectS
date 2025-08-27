// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BaseUIWidget.h"
#include "UI/EEnemyRange.h"
#include "DamageIndicatorWidget.generated.h"


class ASuraPawnPlayer;
class UImage;

UCLASS()
class SURAS_API UDamageIndicatorWidget : public UBaseUIWidget
{
	GENERATED_BODY()

	// 내부 상태를 저장하는 변수들. 외부 노출 불필요.
	UPROPERTY(Transient)
	EEnemyRange LastPlayerHitEnemyRange;
	UPROPERTY(Transient)
	FVector LastPlayerHitEnemyPosition;
	UPROPERTY(Transient)
	FVector LastHitRelativeDirection;

	// UI 표시 및 숨기기 타이머를 관리할 핸들
	FTimerHandle IndicatorUpdateTimerHandle; // 기존 변수 이름 변경
	FTimerHandle HideTimerHandle;          // 새로운 변수 추가

	UPROPERTY(EditAnywhere, Category = "UI Settings")
	float IndicatorRadius = 50.0f; // default

	UPROPERTY()
	ASuraPawnPlayer* PlayerPawn;
public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdateEnemyRangeType(EEnemyRange EnemyRangeType);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void HandleDamageTaken(AActor* DamageCauser);

	// 타이머에 의해 반복 호출될 함수: UI를 실시간으로 업데이트합니다.
	void UpdateIndicatorDirection();

	// 타이머 만료 시 호출될 함수: UI를 숨깁니다.
	void HideIndicator();

protected:
	// 위젯 블루프린트에서 접근할 수 있도록 UI 요소에 바인딩할 변수
	// 근거리 및 원거리용 이미지 위젯 변수
	UPROPERTY(meta = (BindWidget))
	UImage* MeleeIndicatorArrow;

	UPROPERTY(meta = (BindWidget))
	UImage* RangedIndicatorArrow;

	// BeginPlay와 유사한 기능의 함수
	virtual void NativeConstruct() override;
};
