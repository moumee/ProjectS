// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SuraBoss_DataAsset.generated.h"

USTRUCT()
struct FBossMeleeAttack
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="Tag")
	FName AttackAreaTag = FName();

	UPROPERTY(EditAnywhere, Category="Animation", meta=(ToolTip="양팔이 정상인 상태의 공격 몽타주"))
	TObjectPtr<UAnimMontage> BothArmsMontage;
	UPROPERTY(EditAnywhere, Category="Animation", meta=(ToolTip="왼팔이 파괴된 상태의 공격 몽타주"))
	TObjectPtr<UAnimMontage> LeftArmDestroyedMontage;
	UPROPERTY(EditAnywhere, Category="Animation", meta=(ToolTip="오른팔이 파괴된 상태의 공격 몽타주"))
	TObjectPtr<UAnimMontage> RightArmDestroyedMontage;

	UPROPERTY(EditAnywhere, Category="Cooldown")
	float Cooldown = 5.f;
};

USTRUCT()
struct FBossRangedAttack
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="Animation")
	TObjectPtr<UAnimMontage> RangedAttackMontage;

	UPROPERTY(EditAnywhere, Category="Cooldown")
	float Cooldown = 5.f;

	UPROPERTY(EditAnywhere, Category="Duration")
	float TargetingDuration = 2.f;
};

USTRUCT()
struct FBossHealth
{
	GENERATED_BODY()
	// The sum of part health should exceed total health.
	UPROPERTY(EditAnywhere, Category="Health | Total")
	float TotalHealth = 1000.f;
	UPROPERTY(EditAnywhere, Category="Health | Parts")
	float LeftArmHealth = 500.f;
	UPROPERTY(EditAnywhere, Category="Health | Parts")
	float RightArmHealth = 500.f;
	UPROPERTY(EditAnywhere, Category="Health | Parts")
	float HeadHealth = 1000.f;
	UPROPERTY(EditAnywhere, Category="Health | Parts")
	float BodyHealth = 1000.f;
};

/**
 * 
 */
UCLASS()
class SURAS_API USuraBoss_DataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, Category="Melee Attack")
	TArray<FBossMeleeAttack> BossMeleeAttacks;

	UPROPERTY(EditAnywhere, Category="Ranged Attack")
	FBossRangedAttack BossRangedAttack;

	UPROPERTY(EditAnywhere, Category="Boss Health")
	FBossHealth BossHealth;

	UPROPERTY(EditAnywhere, Category="Damage", meta=(ToolTip="머리 치명타 데미지 배율"))
	float HeadDamageMultiplier = 1.5f;
};
