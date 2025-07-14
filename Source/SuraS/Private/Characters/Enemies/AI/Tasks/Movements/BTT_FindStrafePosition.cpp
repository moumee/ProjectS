// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/AI/Tasks/Movements/BTT_FindStrafePosition.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemies/SuraCharacterEnemyBase.h"
#include "Characters/Enemies/AI/EnemyBaseAIController.h"
#include "Characters/PawnBasePlayer/SuraPawnPlayer.h"
#include "NavigationSystem.h"
#include "Characters/PawnBasePlayer/SuraPlayerMovementComponent.h"

UBTT_FindStrafePosition::UBTT_FindStrafePosition(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Find Strafe Position";
	
	bCreateNodeInstance = true; // prevent all AI sharing the same instance with the same StrafeRadius value
}

EBTNodeResult::Type UBTT_FindStrafePosition::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (ASuraCharacterEnemyBase* const Enemy = Cast<ASuraCharacterEnemyBase>(OwnerComp.GetAIOwner()->GetCharacter()))
	{
		if (ASuraPawnPlayer* const Player = Cast<ASuraPawnPlayer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("AttackTarget")))
		{
			FVector CurrentLocation = Enemy->GetActorLocation();
			FVector TargetActorLocation;
			if (!Player->GetPlayerMovementComponent()->FindGroundPoint(TargetActorLocation))
				return EBTNodeResult::Failed;

			float Distance = FVector::Dist(TargetActorLocation, CurrentLocation);

			if (Distance >= OwnerComp.GetBlackboardComponent()->GetValueAsFloat("ChaseStrafeRadius"))
			{
				float ChaseStrafeDistance = OwnerComp.GetBlackboardComponent()->GetValueAsFloat("ChaseStrafeRadius");
				ChaseStrafeDistance += FMath::RandRange(-ChaseStrafeRadiusDeviation, ChaseStrafeRadiusDeviation);

				FVector TargetLocation = (CurrentLocation - TargetActorLocation).GetSafeNormal() * ChaseStrafeDistance;

				if (UNavigationSystemV1* const Navigation = UNavigationSystemV1::GetCurrent(GetWorld()))
				{
					FNavLocation RandomLocation;

					if (Navigation->GetRandomPointInNavigableRadius(TargetLocation, LocationSearchRadius, RandomLocation))
					{
						OwnerComp.GetBlackboardComponent()->SetValueAsVector("TargetLocation", RandomLocation);

						FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
						return EBTNodeResult::Succeeded;
					}
				}
			}
			else
			{
				if (UNavigationSystemV1* const Navigation = UNavigationSystemV1::GetCurrent(GetWorld()))
				{
					FNavLocation RandomLocation;

					if (Navigation->GetRandomPointInNavigableRadius(CurrentLocation, LocationSearchRadius, RandomLocation))
					{
						OwnerComp.GetBlackboardComponent()->SetValueAsVector("TargetLocation", RandomLocation);

						FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
						return EBTNodeResult::Succeeded;
					}
				}
			}
		}
	}
	
	return EBTNodeResult::Failed;
}
