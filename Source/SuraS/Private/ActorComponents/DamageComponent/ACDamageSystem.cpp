// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/DamageComponent/ACDamageSystem.h"
#include "Structures/DamageData.h"

// Sets default values for this component's properties
UACDamageSystem::UACDamageSystem()
{
}

void UACDamageSystem::SetHealth(float val)
{
	Health = val;
}

void UACDamageSystem::SetMaxHealth(float val)
{
	MaxHealth = val;
}

void UACDamageSystem::SetIsInterruptible(bool val)
{
	bIsInterruptible = val;
}

void UACDamageSystem::SetIsInvincible(bool val)
{
	bIsInvincible = val;
}

void UACDamageSystem::SetIsDead(bool val)
{
	bIsDead = val;
}

bool UACDamageSystem::TakeDamage(const FDamageData& DamageData, AActor* DamageCauser)
{
	if (!bIsDead && (!bIsInvincible || DamageData.bCanForceDamage))
	{
		Health -= DamageData.DamageAmount;

		if (Health <= 0.f)
		{
			bIsDead = true;
			OnDeath.Broadcast(); // Call death event

			return true;
		}

		OnDamaged.Broadcast(); // Call damaged event

		return true;
	}

	return false;
}

