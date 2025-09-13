// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/DamageComponent/ACBossDamageSystem.h"

#include "Structures/DamageData.h"

#define SURFACE_HEAD SurfaceType6
#define SURFACE_BODY SurfaceType7
#define SURFACE_LEFT_ARM SurfaceType8
#define SURFACE_RIGHT_ARM SurfaceType9

UACBossDamageSystem::UACBossDamageSystem()
{
	MaxHeadHealth = 200.f;
	HeadHealth = MaxHeadHealth;
	MaxBodyHealth = 200.f;
	BodyHealth = MaxBodyHealth;
	MaxLeftArmHealth = 200.f;
	LeftArmHealth = MaxLeftArmHealth;
	MaxRightArmHealth = 200.f;
	RightArmHealth = MaxRightArmHealth;
	
	MaxHealth = MaxHeadHealth + MaxBodyHealth + MaxLeftArmHealth + MaxRightArmHealth;
	Health = MaxHealth;
}

bool UACBossDamageSystem::TakeDamage(const FDamageData& DamageData, AActor* DamageCauser)
{
	float CalculatedDamageAmount = 0;
	switch (DamageData.SurfaceType)
	{
		case SURFACE_HEAD:
			{
				float ClampedHealth = FMath::Clamp(HeadHealth - DamageData.DamageAmount, 0, MaxHeadHealth);
				CalculatedDamageAmount = HeadHealth - ClampedHealth;
				HeadHealth = ClampedHealth;
				
			}
			break;
		case SURFACE_BODY:
			{
				float ClampedHealth = FMath::Clamp(BodyHealth - DamageData.DamageAmount, 0, MaxBodyHealth);
				CalculatedDamageAmount = BodyHealth - ClampedHealth;
				BodyHealth = ClampedHealth;
			}
			break;
		case SURFACE_LEFT_ARM:
			{
				float ClampedHealth = FMath::Clamp(LeftArmHealth - DamageData.DamageAmount, 0, LeftArmHealth);
				CalculatedDamageAmount = LeftArmHealth - ClampedHealth;
				LeftArmHealth = ClampedHealth;
				if (LeftArmHealth <= 0 && !bLeftArmDead)
				{
					bLeftArmDead = true;
					OnBodyPartDestroyed.Broadcast(SURFACE_LEFT_ARM);
				}
			}
			break;
		case SURFACE_RIGHT_ARM:
			{
				float ClampedHealth = FMath::Clamp(RightArmHealth - DamageData.DamageAmount, 0, RightArmHealth);
				CalculatedDamageAmount = RightArmHealth - ClampedHealth;
				RightArmHealth = ClampedHealth;
				if (RightArmHealth <= 0 && !bRightArmDead)
				{
					bRightArmDead = true;
					OnBodyPartDestroyed.Broadcast(SURFACE_RIGHT_ARM);
				}
			}
			break;
		default:
			break;
	}

	FDamageData ConvertedDamageData = DamageData;
	ConvertedDamageData.DamageAmount = CalculatedDamageAmount;
	
	return Super::TakeDamage(ConvertedDamageData, DamageCauser);
}
