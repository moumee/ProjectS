// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/Boss/SuraBossAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"



void ASuraBossAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	RunBehaviorTree(BehaviorTree);

	
}

void ASuraBossAIController::BeginPlay()
{
	Super::BeginPlay();

	if (APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0))
	{
		GetBlackboardComponent()->SetValueAsObject("PlayerPawn", PlayerPawn);
	}
}

