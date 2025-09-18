// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyBaseAIController.h"
#include "EnemyTurretAIController.generated.h"

UCLASS()
class SURAS_API AEnemyTurretAIController : public AEnemyBaseAIController
{
	GENERATED_BODY()

protected:
	virtual void OnTargetSighted(AActor* SeenTarget, FAIStimulus const Stimulus) override;
};
