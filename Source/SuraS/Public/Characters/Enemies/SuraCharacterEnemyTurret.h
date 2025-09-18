// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SuraCharacterEnemyRifle.h"
#include "SuraCharacterEnemyTurret.generated.h"

UCLASS()
class SURAS_API ASuraCharacterEnemyTurret : public ASuraCharacterEnemyRifle
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASuraCharacterEnemyTurret();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
