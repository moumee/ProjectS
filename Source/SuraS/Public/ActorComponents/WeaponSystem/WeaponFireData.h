// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "SuraProjectile.h"
#include "ArmRecoilStruct.h"
#include "WeaponRecoilStruct.h"
#include "WeaponCameraShakeBase.h"

//TODO: Sound

#include "WeaponFireData.generated.h"

/**
 * 
 */
USTRUCT(Atomic, BlueprintType)
struct FWeaponFireData
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ASuraProjectile> ProjectileClass;
	UPROPERTY(EditAnywhere)
	FArmRecoilStruct Armrecoil;
	UPROPERTY(EditAnywhere)
	FWeaponRecoilStruct Recoil;
	UPROPERTY(EditAnywhere)
	USoundBase* FireSound;
	UPROPERTY(EditAnywhere)
	UNiagaraSystem* MuzzleFireEffect;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UWeaponCameraShakeBase> CamShake;
	UPROPERTY(EditAnywhere)
	int32 AmmoCost = 1;
	UPROPERTY(EditAnywhere)
	float MaxAngleOfMultiProjectileSpread = 3.f;
	UPROPERTY(EditAnywhere)
	int32 PelletsNum = 6;
	UPROPERTY(EditAnywhere)
	bool bAllowFireWithInsufficientAmmo = false;

	FWeaponFireData() :
		ProjectileClass(nullptr),
		Armrecoil(),
		Recoil(),
		FireSound(nullptr),
		MuzzleFireEffect(nullptr),
		CamShake(),
		AmmoCost(1),
		MaxAngleOfMultiProjectileSpread(3.f),
		PelletsNum(6),
		bAllowFireWithInsufficientAmmo()
	{
	}

	FWeaponFireData(
		TSubclassOf<class ASuraProjectile> InProjectileClass,
		FArmRecoilStruct InArmRecoilStruct,
		FWeaponRecoilStruct InRecoil,
		USoundBase* InFireSound,
		UNiagaraSystem* InMuzzleFireEffect,
		TSubclassOf<UWeaponCameraShakeBase> InCamShake,
		int32 InAmmoCost,
		float InMaxAngleOfMultiProjectileSpread,
		int32 InPelletsNum,
		bool InbAllowFireWithInsufficientAmmo
	) :
		ProjectileClass(InProjectileClass),
		Armrecoil(InArmRecoilStruct),
		Recoil(InRecoil),
		FireSound(InFireSound),
		MuzzleFireEffect(InMuzzleFireEffect),
		CamShake(InCamShake),
		AmmoCost(InAmmoCost),
		MaxAngleOfMultiProjectileSpread(InMaxAngleOfMultiProjectileSpread),
		PelletsNum(InPelletsNum),
		bAllowFireWithInsufficientAmmo(InbAllowFireWithInsufficientAmmo)
	{
	}
};

UCLASS()
class SURAS_API AWeaponFireDataStruct : public AActor
{
	GENERATED_BODY()
public:
	AWeaponFireDataStruct();
};
