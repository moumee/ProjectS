// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/SuraCharacterEnemyRifle.h"

#include "Weapons/Firearms/SuraFirearmRifle.h"
#include "Weapons/Projectiles/SuraEnemyProjectile.h"

ASuraCharacterEnemyRifle::ASuraCharacterEnemyRifle()
{
	EnemyType = "Rifle";
}

void ASuraCharacterEnemyRifle::BeginPlay()
{
	Super::BeginPlay();

	/*Firearm = GetWorld()->SpawnActorDeferred<ASuraFirearmRifle>(RifleClass, GetActorTransform(), this);
	Firearm->InitializeFirearem(this, 150, 30);
	Firearm->FinishSpawning(GetActorTransform(), true);

	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	Firearm->AttachToComponent(GetMesh(), AttachmentRules, FName(TEXT("Gun")));

	EnemyWeapon = Firearm;*/
}

void ASuraCharacterEnemyRifle::Attack(const ASuraPawnPlayer* Player)
{
	if (ProjectileClass)
	{
		const FVector SpawnLocation = GetMesh()->GetSocketLocation(FName(TEXT("Hole")));
		const FRotator SpawnRotation = (Player->GetActorLocation() - SpawnLocation).Rotation();

		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ASuraEnemyProjectile* Projectile = GetWorld()->SpawnActor<ASuraEnemyProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);

		Projectile->SetOwner(this);
		Projectile->LaunchProjectile();
	}
	
	/*if (!AttackAnimations.IsEmpty())
	{
		UAnimInstance* const EnemyAnimInstance = GetMesh()->GetAnimInstance();
		EnemyAnimInstance->Montage_Play(GetRandomAnimationMontage(AttackAnimations));

		// Firearm->Fire(this, Player);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("not found"));
	}*/
}