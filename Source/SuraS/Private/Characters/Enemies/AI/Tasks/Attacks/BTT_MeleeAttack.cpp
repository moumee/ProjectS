// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/AI/Tasks/Attacks/BTT_MeleeAttack.h"

#include "Characters/PawnBasePlayer/SuraPawnPlayer.h"
#include "Characters/Enemies/SuraCharacterEnemyBase.h"
#include "Characters/Enemies/AI/EnemyBaseAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_MeleeAttack::UBTT_MeleeAttack(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Melee Attack";

	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTT_MeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (ASuraCharacterEnemyBase* const Enemy = Cast<ASuraCharacterEnemyBase>(OwnerComp.GetAIOwner()->GetCharacter()))
	{
		OnAttackMontageEnded.BindUObject(this, &UBTT_MeleeAttack::OnAttackEnded, &OwnerComp);

		UAnimInstance* const EnemyAnimInstance = Enemy->GetMesh()->GetAnimInstance();
		UAnimMontage* ChosenMontage = Enemy->ChooseRandomAttackMontage();

		EnemyAnimInstance->Montage_Play(ChosenMontage);
		// Enemy->Attack(Player);

		// EnemyAnimInstance->Montage_SetBlendingOutDelegate(OnAttackMontageEnded, ChosenMontage, OwnerComp); // montage interrupted
		EnemyAnimInstance->Montage_SetEndDelegate(OnAttackMontageEnded, ChosenMontage); // montage ended
	}
	
	return EBTNodeResult::InProgress;
}

void UBTT_MeleeAttack::OnAttackEnded(UAnimMontage* AnimMontage, bool bInterrupted, UBehaviorTreeComponent* OwnerComp)
{
	FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
}
