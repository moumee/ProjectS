// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_Climb.generated.h"

class ASuraCharacterEnemyBase;
/**
 * 
 */
struct FBTTClimbTaskMemory
{
	bool bHasLedgeDetected = false;
	bool bIsDoneClimbing = false;
	
	FVector TargetVelocity = FVector::ZeroVector;
	FRotator TargetRotation = FRotator::ZeroRotator;

	TWeakObjectPtr<ASuraCharacterEnemyBase> CachedEnemy = nullptr;
};

UCLASS()
class SURAS_API UBTT_Climb : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
	void TraceGroundAndWall(uint8* NodeMemory);
	void MoveUpTheLedge(uint8* NodeMemory, FVector ImpactNormal);
	void Move(uint8* NodeMemory, UBehaviorTreeComponent& OwnerComp) const;

protected:
	virtual uint16 GetInstanceMemorySize() const override;

public:
	explicit UBTT_Climb(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
