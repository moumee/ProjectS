// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SuraPlayerMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "SuraPawnPlayerAnimInstance.generated.h"

class ASuraPawnPlayer;
/**
 * 
 */
UCLASS()
class SURAS_API USuraPawnPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<ASuraPawnPlayer> SuraPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EMovementState MovementState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsRunning = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsCrouching = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsDashing = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector2D MovementInputVector;
	
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	
};
