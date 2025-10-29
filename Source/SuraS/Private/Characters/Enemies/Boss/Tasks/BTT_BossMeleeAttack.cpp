// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/Boss/Tasks/BTT_BossMeleeAttack.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemies/Boss/SuraBossAttackArea.h"
#include "Characters/Enemies/Boss/SuraBossStates.h"
#include "Characters/Enemies/Boss/SuraCharacterBossProto.h"
#include "GameFramework/Character.h"

UBTT_BossMeleeAttack::UBTT_BossMeleeAttack()
{
	NodeName = "Boss Melee Attack";
	
	INIT_TASK_NODE_NOTIFY_FLAGS();
}

EBTNodeResult::Type UBTT_BossMeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* OwningPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (!IsValid(OwningPawn)) return EBTNodeResult::Failed;

	ASuraCharacterBossProto* Boss = Cast<ASuraCharacterBossProto>(OwningPawn);

	FBossMeleeAttackMemory* Memory = CastInstanceNodeMemory<FBossMeleeAttackMemory>(NodeMemory);
	check(Memory);
	Memory->Boss = Boss;
	if (!Memory->Boss.IsValid()) return EBTNodeResult::Failed;
	
	Boss->SetCurrentState(EBossState::Attack);

	FName AttackAreaTag = OwnerComp.GetBlackboardComponent()->GetValueAsName(AttackAreaKey.SelectedKeyName);

	TArray<ASuraBossAttackArea*> AttackAreas;
	Boss->GetAttackAreasByTag(AttackAreaTag, AttackAreas);
	if (AttackAreas.IsEmpty()) return EBTNodeResult::Failed;
	
	FBossMeleeInfo MeleeInfo = Boss->GetMeleeAttackMontageAndCooldownByTag(AttackAreaTag);
	Boss->StartMeleeAttackCooldown(MeleeInfo.Cooldown);
	
	UAnimInstance* AnimInstance = Boss->GetMesh()->GetAnimInstance();
	if (!AnimInstance) return EBTNodeResult::Failed;

	FOnMontageEnded OnMontageEndedDelegate;
	OnMontageEndedDelegate.BindUObject(this, &ThisClass::OnMontageEnded, TWeakObjectPtr(&OwnerComp));
	AnimInstance->Montage_Play(MeleeInfo.AttackMontage);
	AnimInstance->Montage_SetEndDelegate(OnMontageEndedDelegate, MeleeInfo.AttackMontage);
	
	return EBTNodeResult::InProgress;
}

void UBTT_BossMeleeAttack::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	EBTNodeResult::Type TaskResult)
{
	FBossMeleeAttackMemory* Memory = CastInstanceNodeMemory<FBossMeleeAttackMemory>(NodeMemory);

	if (ASuraCharacterBossProto* Boss = Memory->Boss.Get())
	{
		if (TaskResult == EBTNodeResult::Succeeded && Boss->GetCurrentState() != EBossState::Dead)
		{
			Boss->SetCurrentState(EBossState::Idle);
		}
	}
}

uint16 UBTT_BossMeleeAttack::GetInstanceMemorySize() const
{
	return sizeof(FBossMeleeAttackMemory);
}

void UBTT_BossMeleeAttack::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	if (UBlackboardData* BBAsset = GetBlackboardAsset())
	{
		AttackAreaKey.ResolveSelectedKey(*BBAsset);
	}
}


void UBTT_BossMeleeAttack::OnMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted, TWeakObjectPtr<UBehaviorTreeComponent> OwnerComp)
{
	if (OwnerComp.IsValid())
	{
		UBehaviorTreeComponent* Component = OwnerComp.Get();
		FinishLatentTask(*Component, bInterrupted ? EBTNodeResult::Failed : EBTNodeResult::Succeeded);
	}
	
}
