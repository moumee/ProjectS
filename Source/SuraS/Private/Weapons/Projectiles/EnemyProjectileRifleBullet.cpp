// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Projectiles/EnemyProjectileRifleBullet.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

AEnemyProjectileRifleBullet::AEnemyProjectileRifleBullet()
{
	ProjectileType = "RifleBullet";
}

void AEnemyProjectileRifleBullet::BeginPlay()
{
	Super::BeginPlay();

	if (SpawnEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAttached(
			SpawnEffect.Get(),
			ProjectileMesh,
			NAME_None,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::KeepRelativeOffset,
			true, false, ENCPoolMethod::AutoRelease);

		GetNiagaraComponent()->Activate();
	}
}

void AEnemyProjectileRifleBullet::ActivateShootingEffect()
{
	if (ShootingEffect)
	{
		GetNiagaraComponent()->Deactivate();
		
		UNiagaraFunctionLibrary::SpawnSystemAttached(
			ShootingEffect.Get(),
			ProjectileMesh,
			NAME_None,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::KeepRelativeOffset,
			true, false, ENCPoolMethod::AutoRelease);

		GetNiagaraComponent()->Activate();
	}
}