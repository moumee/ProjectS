// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_BossRangedFire.generated.h"

class ASuraCharacterBossProto;

struct FBossRangedFireMemory
{
	TWeakObjectPtr<ASuraCharacterBossProto> Boss;
	
	bool IsValid() const { return Boss.IsValid(); }
	void Reset() { Boss.Reset(); }
};

/**
 * 
 */
UCLASS()
class SURAS_API UBTT_BossRangedFire : public UBTTaskNode
{
	GENERATED_BODY()

public:

	UBTT_BossRangedFire();

private:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

	void OnFireMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted, TWeakObjectPtr<UBehaviorTreeComponent> OwnerComp);

	virtual uint16 GetInstanceMemorySize() const override;

	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;
	
};
