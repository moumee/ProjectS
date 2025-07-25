// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemies/SuraCharacterEnemyBase.h"
#include "SuraCharacterEnemyClassShifter.generated.h"

/**
 * 
 */
UCLASS()
class SURAS_API ASuraCharacterEnemyClassShifter : public ASuraCharacterEnemyBase
{
	GENERATED_BODY()
	
	FTimerHandle RotationHandle;

	UPROPERTY()
	const ASuraPawnPlayer* Player;

	void RotateTowardPlayer();
	
protected:
	virtual void BeginPlay() override;

public:
	ASuraCharacterEnemyClassShifter();

	virtual void Attack(const ASuraPawnPlayer* Player) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shape")
	FName DefaultClass = "Melee";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shape")
	FName ChagedClass = "Melee";
};
