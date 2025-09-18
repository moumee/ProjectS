// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/AI/Tasks/Misc/BTT_SetFocus.h"
#include "Characters/Enemies/AI/EnemyBaseAIController.h"
#include "Characters/PawnBasePlayer//SuraPawnPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_SetFocus::UBTT_SetFocus(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Set Focus";
}

EBTNodeResult::Type UBTT_SetFocus::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (ASuraPawnPlayer* const Player = Cast<ASuraPawnPlayer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("AttackTarget")))
	{
		OwnerComp.GetAIOwner()->SetFocus(Player, EAIFocusPriority::Gameplay);

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}

	UE_LOG(LogTemp, Error, TEXT("no enemy controller"));
	return EBTNodeResult::Failed;
}