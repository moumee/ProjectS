// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/DamageComponent/ACBossDamageSystem.h"

#include "Characters/Enemies/Boss/SuraBoss_DataAsset.h"
#include "Structures/DamageData.h"

#define SURFACE_HEAD SurfaceType6
#define SURFACE_BODY SurfaceType7
#define SURFACE_LEFT_ARM SurfaceType8
#define SURFACE_RIGHT_ARM SurfaceType9

UACBossDamageSystem::UACBossDamageSystem()
{
	MaxHeadHealth = 1000.f;
	HeadHealth = MaxHeadHealth;
	MaxBodyHealth = 1000.f;
	BodyHealth = MaxBodyHealth;
	MaxLeftArmHealth = 500.f;
	LeftArmHealth = MaxLeftArmHealth;
	MaxRightArmHealth = 500.f;
	RightArmHealth = MaxRightArmHealth;
	
	MaxHealth = 1000.f;
	Health = MaxHealth;
}

void UACBossDamageSystem::InitializeHealth(const FBossHealth& BossHealth)
{
	MaxHealth = BossHealth.TotalHealth;
	Health = MaxHealth;
	MaxBodyHealth = BossHealth.BodyHealth;
	BodyHealth = MaxBodyHealth;
	MaxHeadHealth = BossHealth.HeadHealth;
	HeadHealth = MaxHeadHealth;
	MaxLeftArmHealth = BossHealth.LeftArmHealth;
	LeftArmHealth = MaxLeftArmHealth;
	MaxRightArmHealth = BossHealth.RightArmHealth;
	RightArmHealth = MaxRightArmHealth;
}

bool UACBossDamageSystem::TakeDamage(const FDamageData& DamageData, AActor* DamageCauser)
{
	float CalculatedDamageAmount = 0;
	switch (DamageData.SurfaceType)
	{
		case SURFACE_HEAD:
			{
				float ClampedHealth = FMath::Max(HeadHealth - DamageData.DamageAmount, 0);
				CalculatedDamageAmount = HeadHealth - ClampedHealth;
				HeadHealth = ClampedHealth;
				
			}
			break;
		case SURFACE_BODY:
			{
				float ClampedHealth = FMath::Max(BodyHealth - DamageData.DamageAmount, 0);
				CalculatedDamageAmount = BodyHealth - ClampedHealth;
				BodyHealth = ClampedHealth;
			}
			break;
		case SURFACE_LEFT_ARM:
			{
				float ClampedHealth = FMath::Max(LeftArmHealth - DamageData.DamageAmount, 0);
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
				float ClampedHealth = FMath::Max(RightArmHealth - DamageData.DamageAmount, 0);
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
