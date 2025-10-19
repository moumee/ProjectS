// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_CheckAttackAreaTag.generated.h"

/**
 * 
 */
UCLASS()
class SURAS_API UBTS_CheckAttackAreaTag : public UBTService
{
	GENERATED_BODY()

public:
	UBTS_CheckAttackAreaTag();

private:

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector AttackAreaTagKey;
	
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	
};
