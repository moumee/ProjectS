// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "Enumerations/Enemies/EnemyEnums.h"
#include "EnemyBaseAIController.generated.h"

/**
 * 
 */
UCLASS()
class SURAS_API AEnemyBaseAIController : public AAIController
{
	GENERATED_BODY()

	UPROPERTY()
	class UAISenseConfig_Sight* SightConfig;

	void SetupPerceptionSystem();

	UFUNCTION()
	void OnTargetSighted(AActor* SeenTarget, FAIStimulus const Stimulus);

	EEnemyStates _CurrentState;

protected:
	virtual void OnPossess(APawn* PossessedPawn) override;

public:
	AEnemyBaseAIController(FObjectInitializer const& ObjectInitializer);

	void InitializeBlackBoard(float StrafeRadius, float AttackRadius, float AttackRate);

	FORCEINLINE EEnemyStates GetCurrentState() const { return _CurrentState; }

	void UpdateCurrentState(EEnemyStates NewState);

	void SetStateToChaseOrAttacking(AActor* TargetActor);
};
