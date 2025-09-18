// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/Boss/Tasks/BTT_BossAttack.h"

#include "AIController.h"
#include "Characters/Enemies/Boss/SuraBossStates.h"
#include "Characters/Enemies/Boss/SuraCharacterBossProto.h"
#include "GameFramework/Character.h"



EBTNodeResult::Type UBTT_BossAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;
	ACharacter* BossCharacter = AIController->GetCharacter();
	if (!BossCharacter) return EBTNodeResult::Failed;
	UAnimInstance* AnimInstance = BossCharacter->GetMesh()->GetAnimInstance();
	if (!AnimInstance) return EBTNodeResult::Failed;

	if (!BossRef) BossRef = Cast<ASuraCharacterBossProto>(BossCharacter);

	if (BossRef)
	{
		BossRef->SetCurrentState(EBossState::Attack);
	}
	
	OnMontageEndedDelegate.BindUObject(this, &ThisClass::OnMontageEnded, &OwnerComp);
	AnimInstance->Montage_Play(AttackMontage);
	AnimInstance->Montage_SetEndDelegate(OnMontageEndedDelegate, AttackMontage);
	
	return EBTNodeResult::InProgress;
}

void UBTT_BossAttack::OnMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted, UBehaviorTreeComponent* OwnerComp)
{
	if (BossRef && !bInterrupted && BossRef->GetCurrentState() != EBossState::Dead)
	{
		BossRef->SetCurrentState(EBossState::Idle);
	}
	FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
}