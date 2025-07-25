// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/SuraCharacterEnemyClassShifter.h"
#include "Structures/Enemies/EnemyAttributesData.h"
#include "Kismet/KismetMathLibrary.h"

ASuraCharacterEnemyClassShifter::ASuraCharacterEnemyClassShifter()
{
	EnemyType = "Melee";
}

void ASuraCharacterEnemyClassShifter::BeginPlay()
{
	Super::BeginPlay();
}

void ASuraCharacterEnemyClassShifter::RotateTowardPlayer()
{
	float NewRotationYaw = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Player->GetActorLocation()).Yaw;

	SetActorRotation(FMath::RInterpTo(GetActorRotation(), FRotator(GetActorRotation().Pitch, NewRotationYaw, GetActorRotation().Roll), GetWorld()->GetDeltaSeconds(), 15.f));
}

void ASuraCharacterEnemyClassShifter::Attack(const ASuraPawnPlayer* AttackTarget)
{
	Player = AttackTarget;

	// Rotate to face the player for attacks
	GetWorldTimerManager().SetTimer(
		RotationHandle,
		this, &ASuraCharacterEnemyClassShifter::RotateTowardPlayer,
		0.01f,
		true
	);
	FTimerHandle ClearTimerHandle;

	GetWorldTimerManager().SetTimer(
		ClearTimerHandle,
		FTimerDelegate::CreateLambda([&]() { GetWorldTimerManager().ClearTimer(RotationHandle); }),
		1.f,
		false
	);
}