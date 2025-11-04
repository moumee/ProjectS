// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_BossRangedTargeting.generated.h"

class ASuraCharacterBossProto;


struct FBossRangedTargetingMemory
{
	TWeakObjectPtr<ASuraCharacterBossProto> Boss;
	TWeakObjectPtr<AActor> TargetActor;

	float TargetingDuration = 2.f;
	float RangedAttackStartTime = 0.f;
};
/**
 * 
 */
UCLASS()
class SURAS_API UBTT_BossRangedTargeting : public UBTTaskNode
{
	GENERATED_BODY()

public:

	UBTT_BossRangedTargeting();

private:

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector TargetKey;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	virtual uint16 GetInstanceMemorySize() const override;
	
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;
};
