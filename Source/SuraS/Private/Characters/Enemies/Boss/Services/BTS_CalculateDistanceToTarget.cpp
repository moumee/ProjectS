// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/Boss/Services/BTS_CalculateDistanceToTarget.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void UBTS_CalculateDistanceToTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	AAIController* AIController = OwnerComp.GetAIOwner();
	APawn* OwnerPawn = AIController ? AIController->GetPawn() : nullptr;

	if (!OwnerPawn)
	{
		return;
	}

	AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetKey.SelectedKeyName));
	if (!TargetActor)
	{
		BlackboardComp->SetValueAsFloat(DistanceKey.SelectedKeyName, FLT_MAX);
		return;
	}

	const float DistanceToTarget = FVector::Distance(TargetActor->GetActorLocation(), OwnerPawn->GetActorLocation());
	BlackboardComp->SetValueAsFloat(DistanceKey.SelectedKeyName, DistanceToTarget);
	
}
