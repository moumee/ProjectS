// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/ANS/ANS_SpawnEnemyProjectile.h"

#include "Characters/Enemies/SuraCharacterEnemyRifle.h"

void UANS_SpawnEnemyProjectile::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                            float TotalDuration, const FAnimNotifyEventReference& EvetnRef)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EvetnRef);

	CachedEnemy = Cast<ASuraCharacterEnemyRifle>(MeshComp->GetOwner());

	if (CachedEnemy	&& CachedEnemy->GetWorld() && CachedEnemy->GetWorld()->IsGameWorld())
		CachedEnemy->SpawnProjectile();

	ANSDuration = EvetnRef.GetNotify()->GetDuration();
	CurrentDuration = 0.f;
}

void UANS_SpawnEnemyProjectile::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EvetnRef)
{
	Super::NotifyTick(MeshComp, Animation, TotalDuration, EvetnRef);

	CurrentDuration += TotalDuration;

	// UE_LOG(LogTemp, Error, TEXT("ANS Current Time / Full Duration: %ff"), CurrentDuration / ANSDuration);

	if (CurrentDuration > ANSDuration && CachedEnemy	&& CachedEnemy->GetWorld() && CachedEnemy->GetWorld()->IsGameWorld())
		CachedEnemy->SetProjectilScale(FMath::Clamp(CurrentDuration / ANSDuration, 0.f, 1.f));
}