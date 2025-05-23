// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

UENUM(BlueprintType)
enum class EDamageType : uint8
{
	None UMETA(DisplayName = "None"),
	Explosion UMETA(DisplayName = "Explosion"),
	Projectile UMETA(DisplayName = "Projectile"),
	Melee UMETA(DisplayName = "Melee")
};
