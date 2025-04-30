// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyProjectileAttributesData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct SURAS_API FEnemyProjectileAttributesData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HeadshotAdditionalDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LifeSpan;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InitialSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InitialRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ExplosionRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HomingAccelerationMagnitude;
};
