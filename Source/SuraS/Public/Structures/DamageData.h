// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enumerations/EDamageType.h"
#include "DamageData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct SURAS_API FDamageData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageAmount = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDamageType DamageType = EDamageType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bCanForceDamage = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName BoneName = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector ImpulseDirection = FVector::ZeroVector;
};
