// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Player/SuraPlayerBaseState.h"
#include "SuraPlayerSlidingState.generated.h"

/**
 * 
 */
UCLASS()
class SURAS_API USuraPlayerSlidingState : public USuraPlayerBaseState
{
	GENERATED_BODY()

	FVector CachedSlopeDownwardDirection = FVector::ZeroVector;

	FVector SlideDirection = FVector::ZeroVector;

	float StateEnterSpeed = 0.f;

	float SlideSpeedIncreaseTime = 0.f;

	float SlideSpeedDecreaseTime = 0.f;

	float SlideAdditionalSpeed = 0.f;

	float IncreaseElapsedTime = 0.f;

	float DecreaseElapsedTime = 0.f;

	bool bSpeedIncreasing = true;

	float StartSpeed = 0.f;

	float CurrentSlideSpeed = 0.f;
	
	float CrouchSpeed = 0.f;
	

	FVector TargetSlideDirection = FVector::ZeroVector;

	UPROPERTY()
	APlayerController* PlayerController;

public:

	USuraPlayerSlidingState();

	virtual void EnterState(ASuraCharacterPlayer* Player) override;

	virtual void UpdateState(ASuraCharacterPlayer* Player, float DeltaTime) override;

	virtual void ExitState(ASuraCharacterPlayer* Player) override;

	virtual void Look(ASuraCharacterPlayer* Player, const FVector2D& InputVector) override;
	
};


