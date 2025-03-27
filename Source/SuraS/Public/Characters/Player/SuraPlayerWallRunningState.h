// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Player/SuraPlayerBaseState.h"
#include "SuraPlayerWallRunningState.generated.h"

/**
 * 
 */
UCLASS()
class SURAS_API USuraPlayerWallRunningState : public USuraPlayerBaseState
{
	GENERATED_BODY()

	EWallSide WallRunSide = EWallSide::Left;

	float StateEnterVelocity = 0.f;


	FHitResult WallHit;

	FHitResult FrontWallHit;

	bool bFrontWallFound = false;
	
	float ElapsedTime = 0.f;

	float MaxDuration;

	bool bShouldSlideUp = false;

	bool bShouldSlideDown = false;

	float WallRunEnterZVelocityThreshold = 0.f;

	float WallRunEnterVerticalForce = 0.f;

	float WallRunCameraTiltAngle = 0.f;

	UPROPERTY()
	APlayerController* PlayerController;
	
public:

	bool bShouldRotateCamera = false;

	
	USuraPlayerWallRunningState();

	virtual void EnterState(ASuraCharacterPlayer* Player) override;
	
	void TiltController(ASuraCharacterPlayer* Player, float DeltaTime);

	void SetPlayerWallOffsetLocation(ASuraCharacterPlayer* Player, float DeltaTime);

	virtual void UpdateState(ASuraCharacterPlayer* Player, float DeltaTime) override;

	virtual void ExitState(ASuraCharacterPlayer* Player) override;

	virtual void Look(ASuraCharacterPlayer* Player, const FVector2D& InputVector) override;

	virtual void Move(ASuraCharacterPlayer* Player, const FVector2D& InputVector) override;


};


