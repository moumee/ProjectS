// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_SetStateToChase.generated.h"

/**
 * 
 */
UCLASS()
class SURAS_API UBTT_SetStateToChase : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	explicit UBTT_SetStateToChase(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
