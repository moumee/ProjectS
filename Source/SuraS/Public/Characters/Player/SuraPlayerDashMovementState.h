// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Player/SuraPlayerBaseState.h"
#include "SuraPlayerDashMovementState.generated.h"

/**
 * 
 */
UCLASS()
class SURAS_API USuraPlayerDashMovementState : public USuraPlayerBaseState
{
	GENERATED_BODY()

public:
	
	USuraPlayerDashMovementState();

	
	
	virtual void EnterState(ASuraCharacterPlayer* Player) override;

	virtual void UpdateState(ASuraCharacterPlayer* Player, float DeltaTime) override;

	virtual void Move(ASuraCharacterPlayer* Player, const FVector2D& InputVector) override;

	virtual void Look(ASuraCharacterPlayer* Player, const FVector2D& InputVector) override;

	virtual void StartJumping(ASuraCharacterPlayer* Player) override;

	

	
};
