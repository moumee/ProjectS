// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyAttributesData.generated.h"

/**
 *
 */
USTRUCT(BlueprintType)
struct SURAS_API FEnemyAttributesData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxWalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxSightRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SightAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StrafeRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ChaseStrafeRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MeleeAttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MeleeAttackSphereRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackDamageAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UAnimMontage*> HitAnimations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UAnimMontage*> DeathAnimations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UAnimMontage*> AttackAnimations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* ClimbAnimation;
};