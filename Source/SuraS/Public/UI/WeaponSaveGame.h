// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "WeaponSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class SURAS_API UWeaponSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TMap<FName, bool> OwnedWeapons; // 무기 소유 상태 저장
	
};
