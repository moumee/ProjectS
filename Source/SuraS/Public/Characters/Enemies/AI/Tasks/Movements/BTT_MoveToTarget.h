// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_MoveToTarget.generated.h"

/**
 * 
 */
UCLASS()
class SURAS_API UBTT_MoveToTarget : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	UPROPERTY()
	AAIController* CachedController;
};
