// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/AI/Tasks/Movements/BTT_Patrol.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemies/SuraCharacterEnemyBase.h"

UBTT_Patrol::UBTT_Patrol(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Patrol";
}

EBTNodeResult::Type UBTT_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (ASuraCharacterEnemyBase* const Enemy = Cast<ASuraCharacterEnemyBase>(OwnerComp.GetAIOwner()->GetCharacter()))
	{
		if (!Enemy->GetPatrolRoute())
			return EBTNodeResult::Failed;
		
		FVector TargetLocation = Enemy->GetPatrolRoute()->GetCurrentPatrolIndexPosition();
		OwnerComp.GetBlackboardComponent()->SetValueAsVector("TargetLocation", TargetLocation);
		Enemy->GetPatrolRoute()->IncrementPatrolRoute();

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
