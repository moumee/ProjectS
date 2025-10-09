// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HitScreenWidget.generated.h"

class UImage;

/**
 * 
 */
UCLASS()
class SURAS_API UHitScreenWidget : public UUserWidget
{
	GENERATED_BODY()


public:
	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Anim_HitFlash;

	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> Anim_Breathing;

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UImage> BackgroundImage;
	
	
};
