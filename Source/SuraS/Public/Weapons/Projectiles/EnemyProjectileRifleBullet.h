// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/Projectiles/SuraEnemyProjectile.h"
#include "EnemyProjectileRifleBullet.generated.h"

class UNiagaraSystem;
/**
 * 
 */
UCLASS()
class SURAS_API AEnemyProjectileRifleBullet : public ASuraEnemyProjectile
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "NiagaraEffects")
	UNiagaraSystem* SpawnEffect;

	UPROPERTY(EditAnywhere, Category = "NiagaraEffects")
	UNiagaraSystem* ShootingEffect;

	virtual void BeginPlay() override;

public:
	AEnemyProjectileRifleBullet();
};
