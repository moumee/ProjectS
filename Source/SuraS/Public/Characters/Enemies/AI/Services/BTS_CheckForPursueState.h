// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTS_CheckForPursueState.generated.h"

class AEnemyBaseAIController;
class ASuraPawnPlayer;
/**
 * 
 */
UCLASS()
class SURAS_API UBTS_CheckForPursueState : public UBTService_BlackboardBase
{
	GENERATED_BODY()

	UPROPERTY()
	AEnemyBaseAIController* OwnerEnemyAIC = nullptr;

	UPROPERTY()
	ASuraPawnPlayer* TargetPlayer = nullptr;

protected:
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override; // called only once when the node is entered by the BT
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	UBTS_CheckForPursueState();
};
