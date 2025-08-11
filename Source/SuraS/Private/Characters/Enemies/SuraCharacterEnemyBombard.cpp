// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/SuraCharacterEnemyBombard.h"

#include "Weapons/Firearms/SuraFirearmMissileLauncher.h"

ASuraCharacterEnemyBombard::ASuraCharacterEnemyBombard()
{
	EnemyType = "Bombard";
}

void ASuraCharacterEnemyBombard::BeginPlay()
{
	Super::BeginPlay();

	Firearm = GetWorld()->SpawnActorDeferred<ASuraFirearmMissileLauncher>(RifleClass, GetActorTransform(), this);
	Firearm->InitializeFirearem(this, 30, 1);
	Firearm->FinishSpawning(GetActorTransform(), true);

	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	Firearm->AttachToComponent(GetMesh(), AttachmentRules, FName(TEXT("Gun")));

	EnemyWeapon = Firearm;
}

void ASuraCharacterEnemyBombard::Attack(ASuraPawnPlayer* Player)
{
	if (!AttackAnimations.IsEmpty())
	{
		UAnimInstance* EnemyAnimInstance = GetMesh()->GetAnimInstance();
		EnemyAnimInstance->Montage_Play(GetRandomAnimationMontage(AttackAnimations));

		Firearm->Fire(this, Player);
	}
	
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("not found"));
	}
}