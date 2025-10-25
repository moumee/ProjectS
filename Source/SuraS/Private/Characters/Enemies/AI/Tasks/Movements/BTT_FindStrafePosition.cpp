// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/AI/Tasks/Movements/BTT_FindStrafePosition.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemies/SuraCharacterEnemyBase.h"
#include "Characters/Enemies/AI/EnemyBaseAIController.h"
#include "Characters/PawnBasePlayer/SuraPawnPlayer.h"
#include "NavigationSystem.h"
#include "Characters/PawnBasePlayer/SuraPlayerMovementComponent.h"

uint16 UBTT_FindStrafePosition::GetInstanceMemorySize() const
{
	return sizeof(FBTTFindStrafePositionTaskMemory);
}

UBTT_FindStrafePosition::UBTT_FindStrafePosition(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Find Strafe Position";
}

EBTNodeResult::Type UBTT_FindStrafePosition::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FBTTFindStrafePositionTaskMemory* Mem = CastInstanceNodeMemory<FBTTFindStrafePositionTaskMemory>(NodeMemory);
	check(Mem);
	
	if (ASuraCharacterEnemyBase* const Enemy = Cast<ASuraCharacterEnemyBase>(OwnerComp.GetAIOwner()->GetCharacter()))
	{
		if (ASuraPawnPlayer* const Player = Cast<ASuraPawnPlayer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("AttackTarget")))
		{
			Mem->CurrentLocation = Enemy->GetActorLocation();
			Mem->TargetActorLocation;
			if (!Player->GetPlayerMovementComponent()->FindGroundPoint(Mem->TargetActorLocation))
				return EBTNodeResult::Failed;

			float Distance = FVector::Dist(Mem->TargetActorLocation, Mem->CurrentLocation);

			if (Distance >= OwnerComp.GetBlackboardComponent()->GetValueAsFloat("ChaseStrafeRadius"))
			{
				float ChaseStrafeDistance = OwnerComp.GetBlackboardComponent()->GetValueAsFloat("ChaseStrafeRadius");
				ChaseStrafeDistance += FMath::RandRange(-ChaseStrafeRadiusDeviation, ChaseStrafeRadiusDeviation);

				Mem->TargetLocation = (Mem->CurrentLocation - Mem->TargetActorLocation).GetSafeNormal() * ChaseStrafeDistance;

				if (UNavigationSystemV1* const Navigation = UNavigationSystemV1::GetCurrent(GetWorld()))
				{
					FNavLocation RandomLocation;

					if (Navigation->GetRandomPointInNavigableRadius(Mem->TargetLocation, LocationSearchRadius, RandomLocation))
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

					if (Navigation->GetRandomPointInNavigableRadius(Mem->CurrentLocation, LocationSearchRadius, RandomLocation))
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
