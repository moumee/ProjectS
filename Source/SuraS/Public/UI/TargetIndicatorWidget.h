// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TargetIndicatorWidget.generated.h"

class UImage;
class UTextBlock;
/**
 * 
 */
UCLASS()
class SURAS_API UTargetIndicatorWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetOwningActor(AActor* InOwningActor);

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
    
	TWeakObjectPtr<AActor> OwningActor;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* DistanceText = nullptr;

	UPROPERTY(meta = (BindWidget))
	UImage* TargetIcon = nullptr;
};
