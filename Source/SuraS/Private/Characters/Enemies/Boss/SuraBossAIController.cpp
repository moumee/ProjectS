// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/Boss/SuraBossAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"


void ASuraBossAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
}

void ASuraBossAIController::BeginPlay()
{
	Super::BeginPlay();

	check(BehaviorTree);
	RunBehaviorTree(BehaviorTree);

	if (APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0))
	{
		if (Blackboard)
		{
			Blackboard->SetValueAsObject("PlayerPawn", PlayerPawn);
		}
	}
}

