// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EBossState : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Attack UMETA(DisplayName = "Attack"),
	ArmDismember UMETA(DisplayName = "Arm Dismember"),
	Dead UMETA(DisplayName = "Dead"),
};

UENUM(BlueprintType)
enum class EBossArmState : uint8
{
	HasBothArms UMETA(DisplayName = "Has Both Arms"),
	LeftArmDestroyed UMETA(DisplayName = "Left Arm Destroyed"),
	RightArmDestroyed UMETA(DisplayName = "Right Arm Destroyed"),
	BothDestroyed UMETA(DisplayName = "Both Arms Destroyed"),
};
