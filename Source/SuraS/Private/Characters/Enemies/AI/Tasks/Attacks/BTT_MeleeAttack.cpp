// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/AI/Tasks/Attacks/BTT_MeleeAttack.h"

#include "ActorComponents/AttackComponents/ACPlayerAttackTokens.h"
#include "Characters/PawnBasePlayer/SuraPawnPlayer.h"
#include "Characters/Enemies/SuraCharacterEnemyBase.h"
#include "Characters/Enemies/AI/EnemyBaseAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_MeleeAttack::UBTT_MeleeAttack(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Melee Attack";
	bNotifyTick = true;
}

EBTNodeResult::Type UBTT_MeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (ASuraCharacterEnemyBase* const Enemy = Cast<ASuraCharacterEnemyBase>(OwnerComp.GetAIOwner()->GetCharacter()))
	{
		if (ASuraPawnPlayer* const Player = Cast<ASuraPawnPlayer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("AttackTarget")))
		{
			OnAttackMontageEnded.BindUObject(this, &UBTT_MeleeAttack::OnAttackEnded);

			IsAttacking = true;
			Target = Player;
			Enemy->Attack(Player);

			Enemy->GetMesh()->GetAnimInstance()->Montage_SetBlendingOutDelegate(OnAttackMontageEnded); // montage interrupted
			Enemy->GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(OnAttackMontageEnded); // montage ended

			FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
			return EBTNodeResult::InProgress;
		}
	}

	return EBTNodeResult::Failed;
}

void UBTT_MeleeAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (!IsAttacking)
	{
		// Target->GetAttackTokensComponent()->ReturnAttackToken(1);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
		
}

void UBTT_MeleeAttack::OnAttackEnded(UAnimMontage* AnimMontage, bool bInterrupted)
{
	IsAttacking = false;
}
