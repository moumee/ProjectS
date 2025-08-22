// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/SuraCharacterEnemyRifle.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "Structures/Enemies/EnemyAttributesData.h"
#include "Weapons/Firearms/SuraFirearmRifle.h"
#include "Weapons/Projectiles/EnemyProjectileRifleBullet.h"

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

void ASuraCharacterEnemyRifle::SpawnProjectile()
{
	// UE_LOG(LogTemp, Warning, TEXT("Spawn Enemy Projectile"));
	
	if (ProjectileClass)
	{
		const FVector SpawnLocation = GetMesh()->GetSocketLocation(FName(TEXT("Hole")));

		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		Projectile = GetWorld()->SpawnActor<AEnemyProjectileRifleBullet>(ProjectileClass, SpawnLocation, GetActorRotation(), ActorSpawnParams);

		Projectile->SetOwner(this);

		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
		Projectile->AttachToComponent(GetMesh(), AttachmentRules, FName("Hole"));
	}
}

void ASuraCharacterEnemyRifle::SetProjectileScale(float scale)
{
	if (Projectile)
	{
		Projectile->SetActorScale3D(Projectile->GetActorScale() * scale);
	}
}

void ASuraCharacterEnemyRifle::Attack(ASuraPawnPlayer* Player)
{
	if (Projectile)
	{
		FDetachmentTransformRules DetachmentRules(EDetachmentRule::KeepWorld, true);
		Projectile->DetachFromActor(DetachmentRules);
		
		Projectile->ActivateShootingEffect();
		
		FVector LaunchVelocity;
		float TimeToTarget = FVector::Dist(GetActorLocation(), Player->GetActorLocation()) / Projectile->GetProjectileMovement()->InitialSpeed;
		
		UGameplayStatics::SuggestProjectileVelocity_MovingTarget(this, LaunchVelocity, Projectile->GetActorLocation(), Player, FVector(0.f, 0.f, 40.f), 0.f, TimeToTarget);
		
		Projectile->LaunchProjectileWithVelocity(LaunchVelocity);
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

void ASuraCharacterEnemyRifle::OnDeathTriggered()
{
	Super::OnDeathTriggered();

	if (Projectile)
	{
		Projectile->Destroy();
	}
}