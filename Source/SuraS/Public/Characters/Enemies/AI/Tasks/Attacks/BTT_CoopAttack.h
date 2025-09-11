// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_CoopAttack.generated.h"

/**
 * 
 */
UCLASS()
class SURAS_API UBTT_CoopAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	FOnMontageEnded OnAttackMontageEnded;

	void OnAttackEnded(UAnimMontage* AnimMontage, bool bInterrupted, UBehaviorTreeComponent* OwnerComp);
	
public:
	explicit UBTT_CoopAttack(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
