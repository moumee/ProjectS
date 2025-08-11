// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_Fire.generated.h"

/**
 * 
 */
UCLASS()
class SURAS_API UBTT_Fire : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	FOnMontageEnded OnAttackMontageEnded;

	bool IsAttacking = false;

	void OnAttackEnded(UAnimMontage* AnimMontage, bool bInterrupted);
	
public:
	explicit UBTT_Fire(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
