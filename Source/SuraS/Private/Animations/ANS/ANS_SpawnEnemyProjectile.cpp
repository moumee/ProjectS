// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/ANS/ANS_SpawnEnemyProjectile.h"

#include "Characters/Enemies/SuraCharacterEnemyRifle.h"

void UANS_SpawnEnemyProjectile::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                            float TotalDuration, const FAnimNotifyEventReference& EvetnRef)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EvetnRef);

	if (ASuraCharacterEnemyRifle* OwnerRangedEnemy = Cast<ASuraCharacterEnemyRifle>(MeshComp->GetOwner()))
	{
		if (OwnerRangedEnemy->GetWorld() && OwnerRangedEnemy->GetWorld()->IsGameWorld())
			OwnerRangedEnemy->SpawnProjectile();

		OwnerRangedEnemy->SetFireANSDuration(EvetnRef.GetNotify()->GetDuration());
		OwnerRangedEnemy->SetCurrentANSTime(0.f);
	}
}

void UANS_SpawnEnemyProjectile::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EvetnRef)
{
	Super::NotifyTick(MeshComp, Animation, TotalDuration, EvetnRef);

	if (ASuraCharacterEnemyRifle* OwnerRangedEnemy = Cast<ASuraCharacterEnemyRifle>(MeshComp->GetOwner()))
	{
		OwnerRangedEnemy->SetCurrentANSTime(OwnerRangedEnemy->GetCurrentANSTime() + TotalDuration);
		
		if (OwnerRangedEnemy->GetWorld() && OwnerRangedEnemy->GetWorld()->IsGameWorld() && OwnerRangedEnemy->GetCurrentANSTime() < OwnerRangedEnemy->GetFireANSDuration())
			OwnerRangedEnemy->SetProjectileScale(FMath::Clamp(OwnerRangedEnemy->GetCurrentANSTime() / OwnerRangedEnemy->GetFireANSDuration(), 0.f, 1.f));

		// UE_LOG(LogTemp, Error, TEXT("ANS Current Time / Full Duration: %ff"), OwnerRangedEnemy->GetCurrentANSTime() / OwnerRangedEnemy->GetFireANSDuration());
	}
}