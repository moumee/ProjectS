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
	float DamageAmount = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HeadshotAdditionalDamage = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LifeSpan = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InitialSpeed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxSpeed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InitialRadius = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ExplosionRadius = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HomingAccelerationMagnitude = 0.f;
};
