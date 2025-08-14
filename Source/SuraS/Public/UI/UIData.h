// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h" 
#include "UIData.generated.h" 

USTRUCT(BlueprintType)
struct SURAS_API FUIData : public FTableRowBase
{
	GENERATED_BODY()

public:
	// UI 요소 이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SettingValues")
	float AnimDuration;
};
