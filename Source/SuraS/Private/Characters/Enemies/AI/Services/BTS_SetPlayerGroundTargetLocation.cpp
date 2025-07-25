// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/AI/Services/BTS_SetPlayerGroundTargetLocation.h"

#include "Characters/Enemies/AI/EnemyBaseAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemies/SuraCharacterEnemyBase.h"
#include "Characters/PawnBAsePlayer/SuraPawnPlayer.h"
#include "Characters/PawnBasePlayer/SuraPlayerMovementComponent.h"

UBTS_SetPlayerGroundTargetLocation::UBTS_SetPlayerGroundTargetLocation()
{
	NodeName = "Set Player Ground Target Location";
	bNotifyBecomeRelevant = true;
	bNotifyTick = true;
}

void UBTS_SetPlayerGroundTargetLocation::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	if (ASuraCharacterEnemyBase* const Enemy = Cast<ASuraCharacterEnemyBase>(OwnerComp.GetAIOwner()->GetCharacter()))
		OwnerEnemy = Enemy;

	if (ASuraPawnPlayer* const Player = Cast<ASuraPawnPlayer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("AttackTarget")))
		TargetPlayer = Player;
}

void UBTS_SetPlayerGroundTargetLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
	float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (OwnerEnemy && TargetPlayer)
	{
		FVector PlayerGroundLocation = FVector::ZeroVector;
		
		if (TargetPlayer->GetPlayerMovementComponent()->FindGroundPoint(PlayerGroundLocation))
			OwnerComp.GetBlackboardComponent()->SetValueAsVector("TargetLocation", PlayerGroundLocation);
	}
}
