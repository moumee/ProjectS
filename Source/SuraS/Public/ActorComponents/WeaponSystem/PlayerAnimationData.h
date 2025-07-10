// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PlayerAnimationData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct SURAS_API FPlayerAnimationData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpringDamper")
	FVector Stiffness;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpringDamper")
	FVector Damping;
	//--------------------------------------------------------------
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	FRotator RotationWhenCrouching_Hand_R;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	FRotator RotationWhenCrouching_LowerArm_R;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	FRotator RotationWhenCrouching_UpperArm_R;
};
