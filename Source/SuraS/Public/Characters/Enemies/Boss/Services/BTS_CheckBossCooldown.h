// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_CheckBossCooldown.generated.h"

/**
 * 
 */
UCLASS()
class SURAS_API UBTS_CheckBossCooldown : public UBTService
{
	GENERATED_BODY()

public:

	UBTS_CheckBossCooldown();

private:

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector MeleeCooldownKey;
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector RangedCooldownKey;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
};
