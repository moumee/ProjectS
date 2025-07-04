// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/AI/Tasks/StateChanges/BTT_SetStateToChase.h"

#include "ActorComponents/AttackComponents/ACPlayerAttackTokens.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemies/AI/EnemyBaseAIController.h"
#include "Characters/PawnBasePlayer/SuraPawnPlayer.h"

UBTT_SetStateToChase::UBTT_SetStateToChase(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Set State to Chase";
}

EBTNodeResult::Type UBTT_SetStateToChase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AEnemyBaseAIController* const EnemyController = Cast<AEnemyBaseAIController>(OwnerComp.GetAIOwner()))
	{
		if (EnemyController->GetCurrentState() == EEnemyStates::Pursue || EnemyController->GetCurrentState() == EEnemyStates::Attacking)
			EnemyController->EndPursueState();

		EnemyController->UpdateCurrentState(EEnemyStates::Chase);

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}

