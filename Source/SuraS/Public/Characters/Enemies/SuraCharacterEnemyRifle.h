// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemies/SuraCharacterEnemyBase.h"
#include "SuraCharacterEnemyRifle.generated.h"

class AEnemyProjectileRifleBullet;
// class ASuraFirearmRifle;
/**
 * 
 */
UCLASS()
class SURAS_API ASuraCharacterEnemyRifle : public ASuraCharacterEnemyBase
{
	GENERATED_BODY()

	UPROPERTY()
	AEnemyProjectileRifleBullet* Projectile;

	float FireANSDuration = 0.f;
	float CurrentANSTime = 0.f;

protected:
	virtual void BeginPlay() override;

public:
	ASuraCharacterEnemyRifle();

	UPROPERTY(EditAnywhere, Category = "Projectile")
	TSubclassOf<class AEnemyProjectileRifleBullet> ProjectileClass;

	/*UPROPERTY(EditAnywhere, Category = "Firearm")
	TSubclassOf<class ASuraFirearmRifle> RifleClass;*/

	void SpawnProjectile();

	void SetProjectileScale(float scale);

	virtual void Attack(ASuraPawnPlayer* Player) override;

	virtual void OnDeathTriggered() override;

	FORCEINLINE float GetFireANSDuration() const { return FireANSDuration; }
	FORCEINLINE float GetCurrentANSTime() const { return CurrentANSTime; }
	FORCEINLINE void SetFireANSDuration(float time) { FireANSDuration = time; };
	FORCEINLINE void SetCurrentANSTime(float time) { CurrentANSTime = time; };
};
