// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_SpawnEnemyProjectile.generated.h"

class ASuraCharacterEnemyRifle;
/**
 * 
 */
UCLASS()
class SURAS_API UANS_SpawnEnemyProjectile : public UAnimNotifyState
{
	GENERATED_BODY()

	UPROPERTY()
	ASuraCharacterEnemyRifle* CachedEnemy;

	float ANSDuration = 0.f;
	float CurrentDuration = 0.f;
	
public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EvetnRef) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EvetnRef) override;
};
