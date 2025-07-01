// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_Strafe.generated.h"

/**
 * 
 */
UCLASS()
class SURAS_API UBTT_Strafe : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	explicit UBTT_Strafe(FObjectInitializer const& ObjectInitializer);
};
