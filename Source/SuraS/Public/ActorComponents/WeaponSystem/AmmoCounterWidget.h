// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AmmoCounterWidget.generated.h"

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
};