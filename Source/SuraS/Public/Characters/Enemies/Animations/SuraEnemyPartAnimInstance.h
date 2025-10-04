// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SuraEnemyPartAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class SURAS_API USuraEnemyPartAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<USkeletalMeshComponent> LeaderMeshComp;

	
public:
	virtual void NativeInitializeAnimation() override;

	UPROPERTY(BlueprintReadOnly)
	bool bIsDead = false;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	FPoseSnapshot LeaderPoseSnapshot;
};
