// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponents/UISystem/ACSkillManager.h"
#include "ActorComponents/UISystem/ACUIMangerComponent.h"
#include "UI/BaseUIWidget.h"
#include "RocketLauncherSkillWidget.generated.h"

class UImage;
class UDataTable;
struct FUIData;
/**
 * 
 */
UCLASS()
class SURAS_API URocketLauncherSkillWidget : public UBaseUIWidget
{
	GENERATED_BODY()

	float ZoomDuration = 0.0f;

	UPROPERTY(meta = (BindWidget))
	UImage* SlowScreen;
	
	// 플레이어에서 넘겨줄 데이터테이블
	UPROPERTY()
	UDataTable* DT_UI;
	
	bool bIsZooming = false;
	FTimerHandle ZoomTimerHandle;
	float ElapsedTime = 0.0f;
	FVector2D StartScale;
	FVector2D TargetScale;

	bool bIsFadingOut = false;
	float FadeElapsedTime = 0.0f;
	float FadeDuration = 0.5f; // 애니메이션 지속 시간 (초)
	FVector2D FadeStartScale;
	FVector2D FadeTargetScale = FVector2D(2.0f, 2.0f); // 최종 스케일

	UACSkillManager* SkillManager;


public:
	virtual void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);
	
	// 플레이어 쪽에서 데이터테이블 할당할 때 호출
	void InitUIDataTable(UDataTable* InTable);

	UFUNCTION()
	void RocketLauncherZoomAnimation();
	void SetSKillManager(UACSkillManager* IM);
	
	UFUNCTION()
	void RocketLauncherSkillFadeOut();

	
};
