// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_FindStrafePosition.generated.h"

/**
 * 
 */
UCLASS()
class SURAS_API UBTT_FindStrafePosition : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strafe", meta = (AllowPrivateAccess = "true"))
	float ChaseStrafeRadiusDeviation = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Strafe", meta = (AllowPrivateAccess = "true"))
	float LocationSearchRadius = 0.f;

public:
	explicit UBTT_FindStrafePosition(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
