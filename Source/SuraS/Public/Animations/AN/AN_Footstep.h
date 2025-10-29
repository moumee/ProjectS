// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_Footstep.generated.h"

/**
 * 
 */
UCLASS()
class SURAS_API UAN_Footstep : public UAnimNotify
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> FootstepSound;

	UPROPERTY(EditAnywhere)
	float VolumeMultiplier = 0.05f;

	UPROPERTY(EditAnywhere)
	float PitchMultiplier = 3.f;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
