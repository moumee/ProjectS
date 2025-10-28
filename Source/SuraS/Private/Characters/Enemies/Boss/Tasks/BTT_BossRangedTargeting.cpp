// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/Boss/Tasks/BTT_BossRangedTargeting.h"

#include "AIController.h"
#include "NiagaraComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemies/Boss/SuraCharacterBossProto.h"

UBTT_BossRangedTargeting::UBTT_BossRangedTargeting()
{
	NodeName = "Boss Ranged Targeting";

	INIT_TASK_NODE_NOTIFY_FLAGS();
}

EBTNodeResult::Type UBTT_BossRangedTargeting::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FBossRangedTargetingMemory* Memory = CastInstanceNodeMemory<FBossRangedTargetingMemory>(NodeMemory);
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

	Memory->TargetingDuration = Boss->GetRangedAttackTargetingDuration();
	Memory->RangedAttackStartTime = OwnerComp.GetWorld()->GetTimeSeconds();
	
	Boss->SetCurrentState(EBossState::Attack);
	
	UNiagaraComponent* LaserComp = Boss->GetLaserNiagaraComponent();
	if (!LaserComp) return EBTNodeResult::Failed;
	LaserComp->Activate();
	
	return EBTNodeResult::InProgress;
}

void UBTT_BossRangedTargeting::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FBossRangedTargetingMemory* Memory = CastInstanceNodeMemory<FBossRangedTargetingMemory>(NodeMemory);
	AActor* PlayerActor = Memory->TargetActor.Get();
	ASuraCharacterBossProto* Boss = Memory->Boss.Get();
	if (!PlayerActor || !Boss) FinishLatentTask(OwnerComp, EBTNodeResult::Failed);

	UNiagaraComponent* LaserComp = Boss->GetLaserNiagaraComponent();
	if (!IsValid(LaserComp)) FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	
	LaserComp->SetVariableVec3("User.BeamEnd", PlayerActor->GetActorLocation());

	float CurrentTime = OwnerComp.GetWorld()->GetTimeSeconds();
	if (CurrentTime >= Memory->RangedAttackStartTime + Memory->TargetingDuration)
	{
		Boss->SetLaserFireEnd(PlayerActor->GetActorLocation());
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	
}

uint16 UBTT_BossRangedTargeting::GetInstanceMemorySize() const
{
	return sizeof(FBossRangedTargetingMemory);
}

void UBTT_BossRangedTargeting::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	if (UBlackboardData* BBAsset = GetBlackboardAsset())
	{
		TargetKey.ResolveSelectedKey(*BBAsset);
	}
}

void UBTT_BossRangedTargeting::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTNodeResult::Type TaskResult)
{
	FBossRangedTargetingMemory* Memory = CastInstanceNodeMemory<FBossRangedTargetingMemory>(NodeMemory);
	
	if (TaskResult != EBTNodeResult::Succeeded)
	{
		if (ASuraCharacterBossProto* Boss = Memory->Boss.Get())
		{
			Boss->GetLaserNiagaraComponent()->DeactivateImmediate();
		}
	}
}


