// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_CheckPlayerHealthCondition.generated.h"

/**
 * 
 */
UCLASS()
class SURAS_API UBTT_CheckPlayerHealthCondition : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	explicit UBTT_CheckPlayerHealthCondition(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
