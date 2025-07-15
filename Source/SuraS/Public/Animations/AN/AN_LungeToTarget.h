// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_LungeToTarget.generated.h"

/**
 * 
 */
UCLASS()
class SURAS_API UAN_LungeToTarget : public UAnimNotify
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
    	float LungeForce;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
