// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_Strafe.generated.h"

/**
 * 
 */
struct FBTTStrafeTaskMemory
{
	int32 StrafeDirection = 1;
	float DeltaTime = 0.f;
};

UCLASS()
class SURAS_API UBTT_Strafe : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strafe", meta = (AllowPrivateAccess = "true"))
	float StrafeDuration = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strafe", meta = (AllowPrivateAccess = "true"))
	float StrafeDurationDeviation = 0.f;

protected:
	virtual uint16 GetInstanceMemorySize() const override;

public:
	explicit UBTT_Strafe(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
