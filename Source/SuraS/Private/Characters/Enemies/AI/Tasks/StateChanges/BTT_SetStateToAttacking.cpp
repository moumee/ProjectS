// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/AI/Tasks/StateChanges/BTT_SetStateToAttacking.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemies/AI/EnemyBaseAIController.h"

UBTT_SetStateToAttacking::UBTT_SetStateToAttacking(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Set State to Attacking";
}

EBTNodeResult::Type UBTT_SetStateToAttacking::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AEnemyBaseAIController* const EnemyController = Cast<AEnemyBaseAIController>(OwnerComp.GetAIOwner()))
	{
		EnemyController->UpdateCurrentState(EEnemyStates::Attacking);

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}

