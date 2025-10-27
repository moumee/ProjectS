// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_BossMeleeAttack.generated.h"

class ASuraCharacterBossProto;

struct FBossMeleeAttackMemory
{
	TWeakObjectPtr<ASuraCharacterBossProto> Boss;
};
/**
 * 
 */
UCLASS()
class SURAS_API UBTT_BossMeleeAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:

	UBTT_BossMeleeAttack();

private:
	
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector AttackAreaKey;

	void OnMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted, TWeakObjectPtr<UBehaviorTreeComponent> OwnerComp);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

	virtual uint16 GetInstanceMemorySize() const override;

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	
	
};
