// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/AI/Services/BTS_CalculateDistanceToAttackTarget.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemies/SuraCharacterEnemyBase.h"
#include "Characters/Enemies/AI/EnemyBaseAIController.h"
#include "Characters/PawnBasePlayer/SuraPawnPlayer.h"

void UBTS_CalculateDistanceToAttackTarget::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	if (ASuraCharacterEnemyBase* const Enemy = Cast<ASuraCharacterEnemyBase>(OwnerComp.GetAIOwner()->GetCharacter()))
		OwnerEnemy = Enemy;

	if (ASuraPawnPlayer* const Player = Cast<ASuraPawnPlayer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("AttackTarget")))
		TargetPlayer = Player;
}

void UBTS_CalculateDistanceToAttackTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (OwnerEnemy && TargetPlayer)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsFloat("DistanceToAttackTarget", FVector::Dist(OwnerEnemy->GetActorLocation(), TargetPlayer->GetActorLocation()));
	}
}

UBTS_CalculateDistanceToAttackTarget::UBTS_CalculateDistanceToAttackTarget()
{
	NodeName = "Calculate Distance To Attack Target";
	bNotifyBecomeRelevant = true;
	bNotifyTick = true;
}
