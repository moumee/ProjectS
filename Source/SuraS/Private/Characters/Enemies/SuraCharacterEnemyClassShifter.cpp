// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/SuraCharacterEnemyClassShifter.h"
#include "Structures/Enemies/EnemyAttributesData.h"
#include "ActorComponents/DamageComponent/ACEnemyDamageSystem.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

ASuraCharacterEnemyClassShifter::ASuraCharacterEnemyClassShifter()
{
	EnemyType = DefaultClass;
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

void ASuraCharacterEnemyClassShifter::Attack(ASuraPawnPlayer* AttackTarget)
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

void ASuraCharacterEnemyClassShifter::ClassShiftingInitializeEnemy()
{

		if (const auto EnemyAttributesData = EnemyAttributesDT.DataTable->FindRow<FEnemyAttributesData>(EnemyType, ""))
		{
			GetDamageSystemComp()->SetMaxHealth(EnemyAttributesData->MaxHealth);
			GetDamageSystemComp()->SetImpulsePower(EnemyAttributesData->partsKnockback_Weak,
				EnemyAttributesData->partsKnockback_Normal, EnemyAttributesData->partsKnockback_Hard);
			GetDamageSystemComp()->SetPartsHealth(EnemyAttributesData->HeadHealth, EnemyAttributesData->BodyHealth, EnemyAttributesData -> RArmHealth,
				EnemyAttributesData->LArmHealth, EnemyAttributesData->RLegHealth, EnemyAttributesData->LLegHealth);

			GetCharacterMovement()->MaxWalkSpeed = EnemyAttributesData->MaxWalkSpeed;

			AttackDamageAmount = EnemyAttributesData->AttackDamageAmount;
			MeleeAttackRange = EnemyAttributesData->MeleeAttackRange;
			MeleeAttackSphereRadius = EnemyAttributesData->MeleeAttackSphereRadius;

			HitAnimations = EnemyAttributesData->HitAnimations;
			DeathAnimations = EnemyAttributesData->DeathAnimations;
			AttackAnimations = EnemyAttributesData->AttackAnimations;
			ClimbAnimation = EnemyAttributesData->ClimbAnimation;

			GetMesh()->SetAnimInstanceClass(ChangedAnimBP);
		}
	
}

void ASuraCharacterEnemyClassShifter::SetCrippled()
{
	UE_LOG(LogTemp, Error, TEXT("bonhjgjjvjvj"));
	Super::SetCrippled();
	EnemyType = ChangedClass;
	ClassShiftingInitializeEnemy();
}
