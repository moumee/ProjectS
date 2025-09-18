// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/AI/EnemyTurretAIController.h"

void AEnemyTurretAIController::OnTargetSighted(AActor* SeenTarget, FAIStimulus const Stimulus)
{
	UpdateCurrentState(EEnemyStates::Attacking);
}
