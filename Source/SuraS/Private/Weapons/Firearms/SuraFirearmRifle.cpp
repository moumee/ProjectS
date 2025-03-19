// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Firearms/SuraFirearmRifle.h"
#include "Weapons/Projectiles/SuraEnemyProjectile.h"

void ASuraFirearmRifle::Fire(AActor* FirearmOwner, const AActor* TargetActor, float AdditionalDamage, float AdditionalRadius, bool bIsHoming)
{
	if (ProjectileClass)
	{
		const FVector SpawnLocation = FirearmMesh->GetSocketLocation(FName(TEXT("Muzzle")));
		const FRotator SpawnRotation = (TargetActor->GetActorLocation() - SpawnLocation).Rotation();

		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ASuraEnemyProjectile* Projectile = GetWorld()->SpawnActor<ASuraEnemyProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);

		Projectile->InitializeProjectile(FirearmOwner, AdditionalDamage, AdditionalRadius);
		Projectile->LaunchProjectile();
	}
}