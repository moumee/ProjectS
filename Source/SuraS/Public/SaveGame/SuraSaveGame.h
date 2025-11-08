// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SuraSaveGame.generated.h"

enum class EWeaponName : uint8;
/**
 * 
 */
UCLASS()
class SURAS_API USuraSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	USuraSaveGame();

	UPROPERTY(VisibleAnywhere)
	FTransform SpawnTransform;

	UPROPERTY(visibleAnywhere)
	FName MapName;

	UPROPERTY(VisibleAnywhere)
	int32 CheckpointOrderIndex = 0;

	UPROPERTY()
	TMap<EWeaponName, bool> OwnedWeapons;
	
};
