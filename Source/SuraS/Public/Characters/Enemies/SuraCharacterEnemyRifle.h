// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemies/SuraCharacterEnemyBase.h"
#include "SuraCharacterEnemyRifle.generated.h"

// class ASuraFirearmRifle;
/**
 * 
 */
UCLASS()
class SURAS_API ASuraCharacterEnemyRifle : public ASuraCharacterEnemyBase
{
	GENERATED_BODY()

	/*UPROPERTY()
	ASuraFirearmRifle* Firearm;*/

protected:
	virtual void BeginPlay() override;

public:
	ASuraCharacterEnemyRifle();

	UPROPERTY(EditAnywhere, Category = "Projectile")
	TSubclassOf<class AEnemyProjectileRifleBullet> ProjectileClass;

	/*UPROPERTY(EditAnywhere, Category = "Firearm")
	TSubclassOf<class ASuraFirearmRifle> RifleClass;*/

	virtual void Attack(ASuraPawnPlayer* Player) override;
};
