// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_RotateTowardsAttackArea.generated.h"

/**
 * 
 */
UCLASS()
class SURAS_API UANS_RotateTowardsAttackArea : public UAnimNotifyState
{
	GENERATED_BODY()

	FRotator TargetRotation;

	UPROPERTY(EditAnywhere)
	float RotationInterpSpeed = 5.f;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

	
};
