// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/SuraCharacterEnemyRifle.h"

#include "Kismet/GameplayStatics.h"
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

void ASuraCharacterEnemyRifle::Attack(ASuraPawnPlayer* Player)
{
	if (ProjectileClass)
	{
		const FVector SpawnLocation = GetMesh()->GetSocketLocation(FName(TEXT("Hole")));
		const FRotator SpawnRotation = (Player->GetActorLocation() - SpawnLocation).Rotation();

		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AEnemyProjectileRifleBullet* Projectile = GetWorld()->SpawnActor<AEnemyProjectileRifleBullet>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);

		Projectile->SetOwner(this);

		// UE_LOG(LogTemp, Error, TEXT("Player Velocity: %f, %f, %f"), Player->GetVelocity().X, Player->GetVelocity().Y, Player->GetVelocity().Z);
		
		FVector LaunchVelocity;
		float TimeToTarget = FVector::Dist(GetActorLocation(), Player->GetActorLocation()) / Projectile->GetProjectileMovement()->InitialSpeed;
		
		UGameplayStatics::SuggestProjectileVelocity_MovingTarget(this, LaunchVelocity, SpawnLocation, Player, FVector(0.f, 0.f, 80.f), 0.f, TimeToTarget);
		
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