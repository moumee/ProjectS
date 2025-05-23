// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ACPlayerMovmentData.generated.h"

struct FPlayerMovementData;
/**
 * 
 */
UCLASS()
class SURAS_API UACPlayerMovementData : public UActorComponent
{
	GENERATED_BODY()

private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data Table", meta = (AllowPrivateAccess = "true"))
	UDataTable* PlayerMovementDT;

	FPlayerMovementData* PlayerRow;

protected:
	
	virtual void BeginPlay() override;

public:

	float GetGravityScale() const;
	
	UFUNCTION(BlueprintCallable, Category = "Data")
	float GetWalkSpeed() const;

	UFUNCTION(BlueprintCallable, Category = "Data")
	float GetWalkRunTransitionDuration() const;

	UFUNCTION(BlueprintCallable, Category = "Data")
	float GetWalkDashTransitionDuration() const;

	UFUNCTION(BlueprintCallable, Category = "Data")
	float GetRunDashTransitionDuration() const;

	UFUNCTION(BlueprintCallable, Category = "Data")
	float GetCrouchRunTransitionDuration() const;

	UFUNCTION(BlueprintCallable, Category = "Data")
	float GetCrouchDashTransitionDuration() const;

	UFUNCTION(BlueprintCallable, Category = "Data")
	float GetWalkCrouchTransitionDuration() const;

	UFUNCTION(BlueprintCallable, Category = "Data")
	float GetRunSpeed() const;

	UFUNCTION(BlueprintCallable, Category = "Data")
	float GetCrouchSpeed() const;

	UFUNCTION(BlueprintCallable, Category = "Data")
	float GetPrimaryJumpZSpeed() const;

	UFUNCTION(BlueprintCallable, Category = "Data")
	float GetDoubleJumpZSpeed() const;

	UFUNCTION(BlueprintCallable, Category = "Data")
	float GetDoubleJumpXYSpeed() const;

	UFUNCTION(BlueprintCallable, Category = "Data")
	float GetAirControl() const;

	UFUNCTION(BlueprintCallable, Category = "Data")
	float GetDashEndSpeed() const;

	UFUNCTION(BlueprintCallable, Category = "Data")
	float GetDashImpulseSpeed() const;

	UFUNCTION(BlueprintCallable, Category = "Data")
	float GetDashSpeedDecreaseDuration() const;

	UFUNCTION(BlueprintCallable, Category = "Data")
	int GetDashMaxStack() const;

	UFUNCTION(BlueprintCallable, Category = "Data")
	float GetDashCooldown() const;

	float GetSlidingAdditionalSpeed() const;

	float GetSlidingSpeedIncreaseTime() const;

	float GetSlidingSpeedDecreaseTime() const;

	float GetWallRunningMaxDuration() const;

	float GetMaxCameraFOV() const;

	float GetMaxCameraFOVSpeed() const;

	float GetMaxWallRunSpeed() const;

	float GetWallRunEnterZVelocityThreshold() const;

	float GetWallRunEnterVerticalForce() const;

	float GetWallRunCameraTiltAngle() const;

};
