// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ActorComponents/WeaponSystem/WeaponName.h"
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
	TMap<EWeaponName, bool> OwnedWeapons; // 무기 소유 상태 저장 //<JaeHyeong> FName -> EWeaponName 수정	
};
