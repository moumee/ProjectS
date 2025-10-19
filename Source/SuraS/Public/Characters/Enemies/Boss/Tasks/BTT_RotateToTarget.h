// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_RotateToTarget.generated.h"

struct FRotateToTargetMemory
{
	float YawRateDegPerSec = 0.f;
	
	TWeakObjectPtr<AActor> TargetActor;
	
	TWeakObjectPtr<APawn> OwningPawn;

	bool IsValid() const
	{
		return TargetActor.IsValid() && OwningPawn.IsValid();
	}

	void Reset()
	{
		TargetActor.Reset();
		OwningPawn.Reset();
	}
};

/**
 * 
 */
UCLASS()
class SURAS_API UBTT_RotateToTarget : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_RotateToTarget();

private:

	UPROPERTY(EditAnywhere)
	float MaxYawRate = 360.f;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector TargetKey;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	virtual uint16 GetInstanceMemorySize() const override;

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
};
