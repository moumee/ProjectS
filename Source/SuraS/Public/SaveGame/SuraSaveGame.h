// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SuraSaveGame.generated.h"

UENUM(BlueprintType)
enum class ESuraVideo : uint8
{
	None,
	FirstVideo,
	SecondVideo,
	ThirdVideo,
};

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
	FTransform SpawnTransform = FTransform();

	UPROPERTY(VisibleAnywhere)
	FName MapName = FName();

	UPROPERTY(VisibleAnywhere)
	int32 CheckpointOrderIndex = -1;

	// Deprecated
	UPROPERTY()
	TMap<EWeaponName, bool> OwnedWeapons;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	ESuraVideo PlayedVideo = ESuraVideo::None;
	
};
