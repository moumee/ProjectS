// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/AI/Tasks/Attacks/BTT_Fire.h"

#include "AIController.h"
#include "Characters/PawnBasePlayer/SuraPawnPlayer.h"
#include "Characters/Enemies/SuraCharacterEnemyBase.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_Fire::UBTT_Fire(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Fire";
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTT_Fire::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (ASuraCharacterEnemyBase* const Enemy = Cast<ASuraCharacterEnemyBase>(OwnerComp.GetAIOwner()->GetCharacter()))
	{
		OnAttackMontageEnded.BindUObject(this, &UBTT_Fire::OnAttackEnded, &OwnerComp);

		UAnimInstance* const EnemyAnimInstance = Enemy->GetMesh()->GetAnimInstance();
		UAnimMontage* ChosenMontage = Enemy->ChooseRandomAttackMontage();

		EnemyAnimInstance->Montage_Play(ChosenMontage);
		// Enemy->Attack(Player);

		// EnemyAnimInstance->Montage_SetBlendingOutDelegate(OnAttackMontageEnded); // montage interrupted
		EnemyAnimInstance->Montage_SetEndDelegate(OnAttackMontageEnded, ChosenMontage); // montage ended
	}

	return EBTNodeResult::InProgress;
}

void UBTT_Fire::OnAttackEnded(UAnimMontage* AnimMontage, bool bInterrupted, UBehaviorTreeComponent* OwnerComp)
{
	FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
}
