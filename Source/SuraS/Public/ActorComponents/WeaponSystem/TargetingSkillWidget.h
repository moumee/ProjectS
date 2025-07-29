// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TargetingSkillWidget.generated.h"

class UTextBlock;

UCLASS()
class SURAS_API UTargetingSkillWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* CurrentSkillCoolDown;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* MaxSkillCoolDown;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* ElapsedTimeAfterTargetingStarted;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* MaxTargetingTime;

	void InitializeUI(float NewCurrCoolDown, float NewMaxSkillCoolDown, float NewCurrTargetingTime, float NewMaxTargetingTime);
	void SetTargetingSkillCoolDown(float NewCurrCoolDown);
	void SetDetectionTimeUIVisible(bool bVisible);
	void SetTargetingTimeUI(float NewCurrTargetingTime);
};