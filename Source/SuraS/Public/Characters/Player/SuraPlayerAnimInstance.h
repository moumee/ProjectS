// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SuraPlayerEnums.h"
#include "Animation/AnimInstance.h"
#include "SuraPlayerAnimInstance.generated.h"

class USuraPlayerBaseState;
class ASuraCharacterPlayer;
/**
 * 
 */
UCLASS()
class SURAS_API USuraPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, Category = "Player")
	ASuraCharacterPlayer* Player = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float GroundSpeed = 0.f;
	
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FVector Velocity;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float RunSpeed = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bIsInAir = false;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bCrouchTriggered = false;
	
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float Direction = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float Pitch = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	USuraPlayerBaseState* CurrentState = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	EPlayerState CurrentStateType;

	
	virtual void NativeInitializeAnimation() override;

	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;
	
};
