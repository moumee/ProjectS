// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SettingSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class SURAS_API USettingSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, Category = "Settings")
	float SavedMouseSensitivity;

	USettingSaveGame()
	{
		SavedMouseSensitivity = 0.7f; // default value;
	}
	
};
