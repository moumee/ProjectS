// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/Boss/SuraBossAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"


void ASuraBossAIController::BeginPlay()
{
	Super::BeginPlay();

	RunBehaviorTree(BehaviorTree);

	if (APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0))
	{
		GetBlackboardComponent()->SetValueAsObject("PlayerPawn", PlayerPawn);
	}
	
}
