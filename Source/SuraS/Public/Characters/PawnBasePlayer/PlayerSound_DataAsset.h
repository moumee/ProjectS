// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PlayerSound_DataAsset.generated.h"

/**
 * 
 */
UCLASS()
class SURAS_API UPlayerSound_DataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> NormalHitSound;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> DownedHitSound;
};
