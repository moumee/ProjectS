// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/AI/Tasks/StateChanges/BTT_TrySettingStateToPursue.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemies/AI/EnemyBaseAIController.h"
#include "Characters/PawnBasePlayer/SuraPawnPlayer.h"

UBTT_TrySettingStateToPursue::UBTT_TrySettingStateToPursue(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Try Setting State to Pursue";
}

EBTNodeResult::Type UBTT_TrySettingStateToPursue::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (AEnemyBaseAIController* const EnemyController = Cast<AEnemyBaseAIController>(OwnerComp.GetAIOwner()))
	{
		if (ASuraPawnPlayer* const Player = Cast<ASuraPawnPlayer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("AttackTarget")))
		{
			EnemyController->SetStateToChaseOrPursue(Player);

			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
			return EBTNodeResult::Succeeded;
		}
	}
	
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}

