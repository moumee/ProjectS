// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerInterface.generated.h"

struct FPlayerKeyHoldModifier;
struct FPlayerMovementDataModifier;
enum class EMovementTriggerKey : uint8;
enum class EMovementDataType : uint8;
// This class does not need to be modified.
UINTERFACE()
class UPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SURAS_API IPlayerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void GravityLaunchPlayer(const FVector& Direction, float ForceAmount) = 0;
	virtual void JumpPadLaunchPlayer(float ForceAmount) = 0;
	virtual void RequestMovementDataModification(const TArray<FPlayerMovementDataModifier>& Modifiers) = 0;
	virtual void RequestMovementKeyHoldModification(const TArray<FPlayerKeyHoldModifier>& Modifiers) = 0;
	virtual void RequestResetModification() = 0;
};
