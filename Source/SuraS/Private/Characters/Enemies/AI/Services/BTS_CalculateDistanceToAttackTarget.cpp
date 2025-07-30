// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/AI/Services/BTS_CalculateDistanceToAttackTarget.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemies/SuraCharacterEnemyBase.h"
#include "Characters/Enemies/AI/EnemyBaseAIController.h"
#include "Characters/PawnBasePlayer/SuraPawnPlayer.h"
#include "Characters/PawnBasePlayer/SuraPlayerMovementComponent.h"

UBTS_CalculateDistanceToAttackTarget::UBTS_CalculateDistanceToAttackTarget()
{
	NodeName = "Calculate Distance To Attack Target";
	bNotifyBecomeRelevant = true;
	bNotifyTick = true;

	bCreateNodeInstance = true;
}

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
		FVector PlayerGroundLocation = FVector(TargetPlayer->GetActorLocation().X, TargetPlayer->GetActorLocation().Y, OwnerEnemy->GetActorLocation().Z);
		OwnerComp.GetBlackboardComponent()->SetValueAsFloat("DistanceToAttackTarget", FVector::Dist(OwnerEnemy->GetActorLocation(), PlayerGroundLocation));
		
		
		/*if (TargetPlayer->GetPlayerMovementComponent()->FindGroundPoint(PlayerGroundLocation))
		{
			OwnerComp.GetBlackboardComponent()->SetValueAsFloat("DistanceToAttackTarget", FVector::Dist(OwnerEnemy->GetActorLocation(), PlayerGroundLocation));
			// UE_LOG(LogTemp, Error, TEXT("Player Location: %f, Ground Location: %f"), FVector::Dist(OwnerEnemy->GetActorLocation(), TargetPlayer->GetActorLocation()), FVector::Dist(OwnerEnemy->GetActorLocation(), PlayerGroundLocation));
		}
		else
			UE_LOG(LogTemp, Error, TEXT("Player Movement Component Not Found"));*/
	}
}
