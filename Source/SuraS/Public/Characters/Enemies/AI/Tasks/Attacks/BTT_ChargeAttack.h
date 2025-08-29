// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_ChargeAttack.generated.h"

class ASuraCharacterEnemyCharger;
/**
 * 
 */
UCLASS()
class SURAS_API UBTT_ChargeAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	FOnMontageEnded OnAttackReadyMontageEnded;
	FOnMontageEnded OnAttackMontageEnded;
	FOnMontageEnded OnStunMontageEnded;

	bool bIsAttacking = false;

	UPROPERTY()
	ASuraCharacterEnemyCharger* CachedCharger;

	void OnAttackReadyEnded(UAnimMontage* AnimMontage, bool bInterrupted);

public:
	explicit UBTT_ChargeAttack(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
