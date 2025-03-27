// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SuraPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SURAS_API ASuraPlayerController : public APlayerController
{
	GENERATED_BODY()

	UPROPERTY()
	ASpectatorPawn* SpawnedDebugPawn = nullptr;

	virtual void SetupInputComponent() override;
	
public:

	void ToggleDebugPossession();
};
