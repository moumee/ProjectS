// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_Climb.generated.h"

class ASuraCharacterEnemyBase;
/**
 * 
 */
UCLASS()
class SURAS_API UBTT_Climb : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	bool bHasLedgeDetected = false;
	bool bIsDoneClimbing = false;
	
	FVector TargetVelocity = FVector::ZeroVector;
	FRotator TargetRotation = FRotator::ZeroRotator;

	UPROPERTY()
	TWeakObjectPtr<ASuraCharacterEnemyBase> CachedEnemy = nullptr;
	
	void TraceGroundAndWall();
	void MoveUpTheLedge(FVector ImpactNormal);
	void Move(UBehaviorTreeComponent& OwnerComp) const;

public:
	explicit UBTT_Climb(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
