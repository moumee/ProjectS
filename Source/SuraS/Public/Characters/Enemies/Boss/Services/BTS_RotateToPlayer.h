// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTS_RotateToPlayer.generated.h"

/**
 * 
 */
UCLASS()
class SURAS_API UBTS_RotateToPlayer : public UBTService_BlackboardBase
{
	GENERATED_BODY()

	float YawRateDegPerSec = 0.f;

	UPROPERTY(EditAnywhere)
	float MaxRotationRate = 360.f;
	

	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
