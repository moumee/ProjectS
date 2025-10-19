// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/Boss/Tasks/BTT_BossRangedAttack.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemies/Boss/SuraCharacterBossProto.h"

UBTT_BossRangedAttack::UBTT_BossRangedAttack()
{
	NodeName = "Boss Ranged Attack";

	INIT_TASK_NODE_NOTIFY_FLAGS();
}

EBTNodeResult::Type UBTT_BossRangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FBossRangedAttackMemory* Memory = CastInstanceNodeMemory<FBossRangedAttackMemory>(NodeMemory);
	check(Memory);
	APawn* OwningPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!OwningPawn) return EBTNodeResult::Failed;
	ASuraCharacterBossProto* Boss = Cast<ASuraCharacterBossProto>(OwningPawn);
	Memory->Boss = Boss;

	UObject* TargetObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetKey.SelectedKeyName);
	if (!TargetObject) return EBTNodeResult::Failed;
	AActor* TargetActor = Cast<AActor>(TargetObject);
	Memory->TargetActor = TargetActor;

	if (!Memory->TargetActor.IsValid() || !Memory->Boss.IsValid()) return EBTNodeResult::Failed;
	
	return EBTNodeResult::InProgress;
}

void UBTT_BossRangedAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}

uint16 UBTT_BossRangedAttack::GetInstanceMemorySize() const
{
	return sizeof(FBossRangedAttackMemory);
}

void UBTT_BossRangedAttack::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	if (UBlackboardData* BBAsset = GetBlackboardAsset())
	{
		TargetKey.ResolveSelectedKey(*BBAsset);
	}
}
