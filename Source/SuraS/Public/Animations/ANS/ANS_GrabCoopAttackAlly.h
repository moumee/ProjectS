// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_GrabCoopAttackAlly.generated.h"

class ASuraPawnPlayer;
class ASuraCharacterEnemyBase;
/**
 * 
 */
UCLASS()
class SURAS_API UANS_GrabCoopAttackAlly : public UAnimNotifyState
{
	GENERATED_BODY()

	UPROPERTY()
	ASuraCharacterEnemyBase* CachedEnemy;

	UPROPERTY()
	ASuraCharacterEnemyBase* CachedEnemyAlly;

	UPROPERTY()
	ASuraPawnPlayer* Player;

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EvetnRef) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EvetnRef) override;
};
