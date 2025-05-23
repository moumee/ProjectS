// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Firearms/SuraFirearmRifle.h"
#include "Weapons/Projectiles/SuraEnemyProjectile.h"

void ASuraFirearmRifle::Fire(AActor* FirearmOwner, const AActor* TargetActor)
{
	if (ProjectileClass)
	{
		const FVector SpawnLocation = WeaponMesh->GetSocketLocation(FName(TEXT("Muzzle")));
		const FRotator SpawnRotation = (TargetActor->GetActorLocation() - SpawnLocation).Rotation();

		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ASuraEnemyProjectile* Projectile = GetWorld()->SpawnActor<ASuraEnemyProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);

		Projectile->SetOwner(FirearmOwner);
		Projectile->LaunchProjectile();
	}
}