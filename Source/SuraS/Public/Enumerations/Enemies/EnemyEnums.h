// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

UENUM(BlueprintType)
enum class EEnemyStates : uint8
{
	None UMETA(DisplayName = "None"),
	Passive UMETA(DisplayName = "Passive"),
	Chase UMETA(DisplayName = "Chase"),
	Pursue UMETA(DisplayName = "Pursue"),
	Attacking UMETA(DisplayName = "Attacking"),
	Climbing UMETA(DisplayName = "Climbing"),
	CoopAttacking UMETA(DisplayName = "CoopAttacking"),
	Traverse UMETA(DisplayName = "Traverse"),
	Dead UMETA(DisplayName = "Dead")
};

UENUM(BlueprintType)
enum class EEnemySpeed : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Walk UMETA(DisplayName = "Walk"),
	Jog UMETA(DisplayName = "Jog"),
	Sprint UMETA(DisplayName = "Sprint")
};