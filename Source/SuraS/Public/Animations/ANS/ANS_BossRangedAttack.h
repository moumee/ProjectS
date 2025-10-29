// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_BossRangedAttack.generated.h"

class ASuraCharacterBossProto;
/**
 * 
 */
UCLASS()
class SURAS_API UANS_BossRangedAttack : public UAnimNotifyState
{
	GENERATED_BODY()

private:

	TWeakObjectPtr<ASuraCharacterBossProto> WeakBoss;

	bool bHasHit = false;

	float DamageAmount = 0.f;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
		const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime,
		const FAnimNotifyEventReference& EventReference) override;

	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference) override;

};
