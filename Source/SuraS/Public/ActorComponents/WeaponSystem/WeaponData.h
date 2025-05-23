// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponents/WeaponSystem/WeaponName.h"
#include "ActorComponents/WeaponSystem/WeaponAction.h"
#include "ActorComponents/WeaponSystem/ProjectileType.h"
#include "ActorComponents/WeaponSystem/WeaponRecoilStruct.h"
#include "ActorComponents/WeaponSystem/WeaponCameraShakeBase.h"
#include "ActorComponents/WeaponSystem/ProjectileSpreadValue.h"
#include "NiagaraSystem.h"
#include "WeaponData.generated.h"

USTRUCT(BlueprintType)
struct SURAS_API FWeaponData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Name")
	EWeaponName WeaponName;
	//-----------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	EWeaponAction LeftMouseAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	EWeaponAction RightMouseAction;
	//-----------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	TSubclassOf<class ASuraProjectile> LeftProjectileClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	TSubclassOf<class ASuraProjectile> RightProjectileClass;
	//-----------------------------------------------------------------

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//EProjectileType ProjectileType; //TODO: 2개 선택 가능하도록 해야함
	
	//-----------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	UNiagaraSystem* FireEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	UNiagaraSystem* ChargeEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	FVector ChargeEffectLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	FRotator ChargeEffectRotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	FVector ChargeEffenctScale = { 1.f, 1.f, 1.f };
	//-----------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* FireSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* ChargeSound;
	//-----------------------------------------------------------------

	/** Start Suhyeon  **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* WeaponImage; // 총기 이미지

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsWeaponOwned; // 총기 소유 불값
	
	/** End Suhyeon **/


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reload")
	int32 MaxAmmoPerMag = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reload")
	int32 MaxTotalAmmo = 200.f;
	//-----------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SingleShot")
	float SingleShotDelay = 1.f;
	//-----------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BurstShot")
	float BurstShotDelay = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BurstShot")
	float BurstShotFireRate = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BurstShot")
	int32 BurstShotCount = 3;
	//-----------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FullAutoShot")
	float FullAutoShotFireRate = 1.f;
	//-----------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileSpread")
	FProjectileSpreadValue DefaultSpread;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileSpread")
	FProjectileSpreadValue ZoomSpread;
	//-----------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MultiProjectile")
	bool bEnableMultiProjectile_Left;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MultiProjectile")
	bool bEnableMultiProjectile_Right;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MultiProjectile")
	float MaxAngleOfMultiProjectileSpread = 15.f;
	//-----------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	FWeaponRecoilStruct DefaultRecoil;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	FWeaponRecoilStruct ZoomRecoil;
	//-----------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = "CameraShake")
	TSubclassOf<UWeaponCameraShakeBase> DefaultCameraShakeClass;
	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = "CameraShake")
	TSubclassOf<UWeaponCameraShakeBase> ZoomCameraShakeClass;
	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = "CameraShake")
	TSubclassOf<UWeaponCameraShakeBase> ChargingCameraShakeClass;
	//-----------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	float MissileLaunchDelay = 0.2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	int32 MaxTargetNum = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	float MaxTargetDetectionRadius = 3000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	float MaxTargetDetectionAngle = 80.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	float MaxTargetDetectionTime = 8.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	float TimeToReachMaxTargetDetectionRange = 2.5f;
	//-----------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Charging")
	bool bAutoFireAtMaxChargeTime = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Charging")
	float ChargeTimeThreshold = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Charging")
	float MaxChargeTime = 3.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Charging")
	float ChargingAdditionalDamageBase = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Charging")
	float ChargingAdditionalRecoilAmountPitchBase = 4.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Charging")
	float ChargingAdditionalRecoilAmountYawBase = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Charging")
	float ChargingAdditionalProjectileRadiusBase = 20.f;
	//-----------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Penetration")
	int32 MaxPenetrableObjectsNum_Left = 4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Penetration")
	int32 MaxPenetrableObjectsNum_Right = 4;
};
