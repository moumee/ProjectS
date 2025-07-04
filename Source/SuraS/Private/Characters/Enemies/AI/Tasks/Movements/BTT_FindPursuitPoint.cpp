// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/AI/Tasks/Movements/BTT_FindPursuitPoint.h"

#include "ActorComponents/AttackComponents/ACPlayerAttackTokens.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/PawnBasePlayer/SuraPawnPlayer.h"

UBTT_FindPursuitPoint::UBTT_FindPursuitPoint(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Find Pursuit Point";
}

EBTNodeResult::Type UBTT_FindPursuitPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	int PursuitIndex = OwnerComp.GetBlackboardComponent()->GetValueAsInt("PursuitIndex");

	if (ASuraPawnPlayer* const Player = Cast<ASuraPawnPlayer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("AttackTarget")))
	{
		FVector TargetActorLocation = Player->GetActorLocation();
		float AngleBetween = 360.f / Player->GetAttackTokensComponent()->GetMaxEnemyPursuitTokens();

		float AngleRadians = FMath::DegreesToRadians(PursuitIndex * AngleBetween); // dividing 360 degrees with 8 possible points
		float AttackRadius = OwnerComp.GetBlackboardComponent()->GetValueAsFloat("AttackRadius");

		float PosX = TargetActorLocation.X + AttackRadius * FMath::Cos(AngleRadians);
		float PosY = TargetActorLocation.Y + AttackRadius * FMath::Sin(AngleRadians);
		float PosZ = TargetActorLocation.Z;

		OwnerComp.GetBlackboardComponent()->SetValueAsVector("TargetLocation", FVector(PosX, PosY, PosZ));

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}
