// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemies/SuraCharacterEnemyBase.h"
#include "SuraCharacterEnemyMelee.generated.h"

/**
 * 
 */
UCLASS()
class SURAS_API ASuraCharacterEnemyMelee : public ASuraCharacterEnemyBase
{
	GENERATED_BODY()

	FTimerHandle RotationHandle;

	UPROPERTY()
	const ASuraPawnPlayer* Player;

	void RotateTowardPlayer();
	
protected:
	virtual void BeginPlay() override;

public:
	ASuraCharacterEnemyMelee();

	virtual void Attack(ASuraPawnPlayer* Player) override;
};
