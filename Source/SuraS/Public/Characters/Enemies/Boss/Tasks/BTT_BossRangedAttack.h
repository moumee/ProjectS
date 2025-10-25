// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_BossRangedAttack.generated.h"

class ASuraCharacterBossProto;


struct FBossRangedAttackMemory
{
	TWeakObjectPtr<ASuraCharacterBossProto> Boss;
	TWeakObjectPtr<AActor> TargetActor;
};
/**
 * 
 */
UCLASS()
class SURAS_API UBTT_BossRangedAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:

	UBTT_BossRangedAttack();

private:

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector TargetKey;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	virtual uint16 GetInstanceMemorySize() const override;
	
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
};
