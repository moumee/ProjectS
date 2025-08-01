// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/AI/Services/BTS_CheckForPursueState.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemies/AI/EnemyBaseAIController.h"
#include "Characters/PawnBAsePlayer/SuraPawnPlayer.h"

UBTS_CheckForPursueState::UBTS_CheckForPursueState()
{
	NodeName = "Check For Pursue State";
	bNotifyBecomeRelevant = true;
	bNotifyTick = true;

	bCreateNodeInstance = true;
}

void UBTS_CheckForPursueState::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	if (AEnemyBaseAIController* const EnemyController = Cast<AEnemyBaseAIController>(OwnerComp.GetAIOwner()))
		OwnerEnemyAIC = EnemyController;

	if (ASuraPawnPlayer* const Player = Cast<ASuraPawnPlayer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("AttackTarget")))
		TargetPlayer = Player;
}

void UBTS_CheckForPursueState::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	OwnerEnemyAIC->SetStateToChaseOrPursue(TargetPlayer);
}
