// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/Boss/Tasks/BTT_BossMeleeAttack.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemies/Boss/SuraBossAttackArea.h"
#include "Characters/Enemies/Boss/SuraBossStates.h"
#include "Characters/Enemies/Boss/SuraCharacterBossProto.h"
#include "GameFramework/Character.h"



EBTNodeResult::Type UBTT_BossMeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;
	APawn* Pawn = AIController->GetPawn();
	if (!Pawn) return EBTNodeResult::Failed;

	BossRef = Cast<ASuraCharacterBossProto>(Pawn);
	if (!BossRef.IsValid()) return EBTNodeResult::Failed;

	ASuraCharacterBossProto* Boss = BossRef.Get();
	Boss->SetCurrentState(EBossState::Attack);

	FName AttackAreaTag = OwnerComp.GetBlackboardComponent()->GetValueAsName(AttackAreaKey.SelectedKeyName);

	TArray<ASuraBossAttackArea*> AttackAreas;
	Boss->GetAttackAreasByTag(AttackAreaTag, AttackAreas);
	if (AttackAreas.IsEmpty()) return EBTNodeResult::Failed;
	
	
	FBossMeleeInfo MeleeInfo = Boss->GetMeleeAttackMontageAndCooldownByTag(AttackAreaTag);
	OwnerComp.GetBlackboardComponent()->SetValueAsFloat(MeleeCooldownKey.SelectedKeyName, MeleeInfo.Cooldown);
	
	UAnimInstance* AnimInstance = Boss->GetMesh()->GetAnimInstance();
	if (!AnimInstance) return EBTNodeResult::Failed;
	
	OnMontageEndedDelegate.BindUObject(this, &ThisClass::OnMontageEnded, &OwnerComp);
	AnimInstance->Montage_Play(MeleeInfo.AttackMontage);
	AnimInstance->Montage_SetEndDelegate(OnMontageEndedDelegate, MeleeInfo.AttackMontage);
	
	return EBTNodeResult::InProgress;
}


void UBTT_BossMeleeAttack::OnMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted, UBehaviorTreeComponent* OwnerComp)
{
	if (BossRef.IsValid())
	{
		ASuraCharacterBossProto* Boss = BossRef.Get();
		if (!bInterrupted && Boss->GetCurrentState() != EBossState::Dead)
		{
			Boss->SetCurrentState(EBossState::Idle);
		}
	}
	FinishLatentTask(*OwnerComp, bInterrupted ? EBTNodeResult::Failed : EBTNodeResult::Succeeded);
}
