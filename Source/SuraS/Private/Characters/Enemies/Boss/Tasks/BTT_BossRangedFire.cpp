// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/Boss/Tasks/BTT_BossRangedFire.h"

#include "AIController.h"
#include "NiagaraComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Characters/Enemies/Boss/SuraCharacterBossProto.h"

UBTT_BossRangedFire::UBTT_BossRangedFire()
{
	NodeName = "Boss Ranged Fire";

	INIT_TASK_NODE_NOTIFY_FLAGS();
}

EBTNodeResult::Type UBTT_BossRangedFire::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FBossRangedFireMemory* Memory = CastInstanceNodeMemory<FBossRangedFireMemory>(NodeMemory);
	APawn* OwningPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!OwningPawn) return EBTNodeResult::Failed;
	ASuraCharacterBossProto* Boss = Cast<ASuraCharacterBossProto>(OwningPawn);
	Memory->Boss = Boss;
	if (!Memory->IsValid()) return EBTNodeResult::Failed;

	USkeletalMeshComponent* Mesh = Boss->GetMesh();
	if (!Mesh) return EBTNodeResult::Failed;
	UAnimInstance* AnimInstance = Mesh->GetAnimInstance();
	if (!AnimInstance) return EBTNodeResult::Failed;
	UAnimMontage* RangedMontage = Boss->GetRangedAttackMontage();
	if (!RangedMontage) return EBTNodeResult::Failed;

	FOnMontageEnded OnMontageEndedDelegate;
	OnMontageEndedDelegate.BindUObject(this, &ThisClass::OnFireMontageEnded, TWeakObjectPtr(&OwnerComp));
	AnimInstance->Montage_Play(RangedMontage);
	AnimInstance->Montage_SetEndDelegate(OnMontageEndedDelegate, RangedMontage);
	Boss->StartRangedAttackCooldown();
	
	return EBTNodeResult::InProgress;
}


void UBTT_BossRangedFire::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);
	
}

void UBTT_BossRangedFire::OnFireMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted, TWeakObjectPtr<UBehaviorTreeComponent> OwnerComp)
{
	if (!OwnerComp.IsValid()) return;
	
	UBehaviorTreeComponent* Component = OwnerComp.Get();
	if (APawn* BossPawn = Component->GetAIOwner()->GetPawn())
	{
		if (ASuraCharacterBossProto* Boss = Cast<ASuraCharacterBossProto>(BossPawn))
		{
			if (UNiagaraComponent* LaseComp = Boss->GetLaserNiagaraComponent())
			{
				if (LaseComp->IsActive())
				{
					LaseComp->DeactivateImmediate();
				}
			}
		}
	}
	FinishLatentTask(*Component, bInterrupted ? EBTNodeResult::Aborted : EBTNodeResult::Succeeded);
}

uint16 UBTT_BossRangedFire::GetInstanceMemorySize() const
{
	return sizeof(FBossRangedFireMemory);
}

void UBTT_BossRangedFire::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTNodeResult::Type TaskResult)
{
	FBossRangedFireMemory* Memory = CastInstanceNodeMemory<FBossRangedFireMemory>(NodeMemory);

	if (Memory->Boss.IsValid())
	{
		ASuraCharacterBossProto* Boss = Memory->Boss.Get();
		if (TaskResult == EBTNodeResult::Succeeded && Boss->GetCurrentState() != EBossState::Dead)
		{
			Boss->SetCurrentState(EBossState::Idle);
		}
	}
}

