// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_BossAttack.generated.h"

/**
 * 
 */
UCLASS()
class SURAS_API UBTT_BossAttack : public UBTTaskNode
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> AttackMontage;

	FOnMontageEnded OnMontageEndedDelegate;

	void OnMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted, UBehaviorTreeComponent* OwnerComp);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	
	
	
};
