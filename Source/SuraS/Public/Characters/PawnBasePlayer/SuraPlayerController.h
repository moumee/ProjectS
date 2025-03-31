// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SuraPlayerController.generated.h"

class ASuraPawnPlayer;
/**
 * 
 */
UCLASS()
class SURAS_API ASuraPlayerController : public APlayerController
{
	GENERATED_BODY()

	bool bIsFirstPerson = true;

	UPROPERTY()
	ASuraPawnPlayer* SuraPawnPlayer = nullptr;

	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;
	
public:

	void ToggleDebugPossession();

	
};
