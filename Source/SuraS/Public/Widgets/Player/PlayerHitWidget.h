 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "PlayerHitWidget.generated.h"

/**
 * 
 */
UCLASS()
class SURAS_API UPlayerHitWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Transient, meta = (BindWidgetAnim, AllowPrivateAccess = "true"))
	UWidgetAnimation* FadeInOutAnimation = nullptr;

	virtual void NativeConstruct() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UTexture2D*> HitEffectTextures;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UTexture2D*> HitEffectBackgrounds;

	UPROPERTY(meta = (BindWidget))
	UImage* HitEffect = nullptr;

	UPROPERTY(meta = (BindWidget))
	UImage* HitEffectBackground = nullptr;

public:
	void PlayFadeAnimation();
	void UpdateHitScreen(int32 Stage);
};
