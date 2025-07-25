// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTS_SetPlayerGroundTargetLocation.generated.h"

class ASuraPawnPlayer;
class ASuraCharacterEnemyBase;
/**
 * 
 */
UCLASS()
class SURAS_API UBTS_SetPlayerGroundTargetLocation : public UBTService_BlackboardBase
{
	GENERATED_BODY()

	UPROPERTY()
	ASuraPawnPlayer* TargetPlayer = nullptr;

	UPROPERTY()
	ASuraCharacterEnemyBase* OwnerEnemy = nullptr;

protected:
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override; // called only once when the node is entered by the BT
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	UBTS_SetPlayerGroundTargetLocation();
};
