// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponents/WeaponSystem/ProjectileType.h"
#include "NiagaraSystem.h"
#include "ProjectileData.generated.h"


USTRUCT(BlueprintType)
struct SURAS_API FProjectileData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EProjectileType ProjectileType = EProjectileType::Projectile_Rifle;

	// <Effect>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	UNiagaraSystem* TrailEffect = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	UNiagaraSystem* ImpactEffect = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	UNiagaraSystem* ExplosionEffect = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	UMaterialInterface* HoleDecal = nullptr;
	//--------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* HitSound = nullptr; //TODO: ªË¡¶
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* HitSound_Default = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* HitSound_Metal = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* HitSound_Glass = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* HitSound_Enemy = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* HitSound_Energy = nullptr;
	//--------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float DefaultDamage = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float HeadShotAdditionalDamage = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InitialLifeSpan = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InitialSpeed = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxSpeed = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InitialRadius = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosive")
	bool bIsExplosive = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosive")
	bool bVisualizeExplosionRadius = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosive")
	float MaxExplosiveDamage = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosive")
	float MaxExplosionRadius = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Homing")
	float HomingAccelerationMagnitude = 3000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Penetration")
	bool bCanPenetrate = false;
	//--------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Impulse")
	bool bCanApplyImpulseToEnemy = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Impulse")
	float HitImpulseToEnemy = 100.f;
	//--------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ricochet")
	bool bCanSimpleBounce = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ricochet")
	int32 MaxRicochetCount = 4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ricochet")
	float MinIncidenceAngle = 5.f;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Penetration")
	//int32 NumPenetrableObjects = 4;

	//--------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitScan")
	bool bDebugHitScan = false;
};
