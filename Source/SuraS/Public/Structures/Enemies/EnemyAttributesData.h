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
	float MaxHealth = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxWalkSpeed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxSightRadius = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SightAngle = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StrafeRadius = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ChaseStrafeRadius = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackRadius = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackRate = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MeleeAttackRange = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MeleeAttackSphereRadius = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackDamageAmount = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UAnimMontage*> HitAnimations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UAnimMontage*> DeathAnimations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UAnimMontage*> AttackAnimations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* ClimbAnimation = nullptr;

	UPROPERTY(editAnywhere, BlueprintReadWrite)
	float HeadHealth = 300.f;

	UPROPERTY(editAnywhere, BlueprintReadWrite)
	float BodyHealth = 100.f;

	UPROPERTY(editAnywhere, BlueprintReadWrite)
	float RArmHealth = 100.f;

	UPROPERTY(editAnywhere, BlueprintReadWrite)
	float LArmHealth = 100.f;

	UPROPERTY(editAnywhere, BlueprintReadWrite)
	float RLegHealth = 400.f;

	UPROPERTY(editAnywhere, BlueprintReadWrite)
	float LLegHealth = 400.f;

	UPROPERTY(editAnywhere, BlueprintReadWrite)
	float partsKnockback_Weak = 200;

	UPROPERTY(editAnywhere, BlueprintReadWrite)
	float partsKnockback_Normal = 300;

	UPROPERTY(editAnywhere, BlueprintReadWrite)
	float partsKnockback_Hard = 400;
};