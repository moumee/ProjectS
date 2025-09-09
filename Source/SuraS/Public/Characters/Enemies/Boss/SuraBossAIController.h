// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SuraBossAIController.generated.h"

/**
 * 
 */
UCLASS()
class SURAS_API ASuraBossAIController : public AAIController
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, Category="AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;
	
	virtual void BeginPlay() override;
	
};
