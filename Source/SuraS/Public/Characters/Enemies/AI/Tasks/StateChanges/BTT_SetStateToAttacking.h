// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_SetStateToAttacking.generated.h"

/**
 * 
 */
UCLASS()
class SURAS_API UBTT_SetStateToAttacking : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	explicit UBTT_SetStateToAttacking(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
