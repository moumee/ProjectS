// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemies/SuraCharacterEnemyBase.h"
#include "SuraCharacterEnemyBombard.generated.h"

class ASuraFirearmMissileLauncher;
/**
 * 
 */
UCLASS()
class SURAS_API ASuraCharacterEnemyBombard : public ASuraCharacterEnemyBase
{
	GENERATED_BODY()

	UPROPERTY()
	ASuraFirearmMissileLauncher* Firearm;

protected:
	virtual void BeginPlay() override;

public:
	ASuraCharacterEnemyBombard();

	UPROPERTY(EditAnywhere, Category = "Firearm")
	TSubclassOf<class ASuraFirearmMissileLauncher> RifleClass;

	virtual void Attack(const ASuraCharacterPlayer* Player) override;
};
