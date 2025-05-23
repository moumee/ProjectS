// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/AI/Tasks/Attacks/BTT_CheckAttackTokens.h"
#include "Characters/Player/SuraCharacterPlayer.h"
#include "Characters/Enemies/SuraCharacterEnemyBase.h"
#include "Characters/Enemies/AI/EnemyBaseAIController.h"
#include "ActorComponents/AttackComponents/ACPlayerAttackTokens.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_CheckAttackTokens::UBTT_CheckAttackTokens(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Check Attack Tokens";
	bNotifyTick = true;
}

EBTNodeResult::Type UBTT_CheckAttackTokens::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (ASuraCharacterEnemyBase* const Enemy = Cast<ASuraCharacterEnemyBase>(OwnerComp.GetAIOwner()->GetCharacter()))
	{
		if (ASuraCharacterPlayer* const Player = Cast<ASuraCharacterPlayer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("AttackTarget")))
		{
			if (!Player->GetAttackTokensComponent()->ReserveAttackToken(1))
				return EBTNodeResult::Failed;

			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}