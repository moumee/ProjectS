// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponents/WeaponSystem/WeaponName.h"
#include "ActorComponents/WeaponSystem/WeaponAction.h"
#include "ActorComponents/WeaponSystem/ProjectileType.h"
#include "ActorComponents/WeaponSystem/WeaponRecoilStruct.h"
#include "ActorComponents/WeaponSystem/ArmRecoilStruct.h"
#include "ActorComponents/WeaponSystem/WeaponCameraShakeBase.h"
#include "ActorComponents/WeaponSystem/ProjectileSpreadValue.h"
#include "NiagaraSystem.h"
#include "WeaponData.generated.h"

USTRUCT(BlueprintType)
struct SURAS_API FWeaponData : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Name")
	EWeaponName WeaponName = EWeaponName::WeaponName_Rifle;
	//-----------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Socket")
	FName WeaponSocket = FName();
	//-----------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	bool bIsSkillWeapon = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	bool bAllowNormalFireForSkillWeapon = false;
	//-----------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	EWeaponAction LeftMouseAction = EWeaponAction::WeaponAction_SingleShot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	EWeaponAction RightMouseAction = EWeaponAction::WeaponAction_SingleShot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	EWeaponAction SkillAction = EWeaponAction::WeaponAction_SkillToggle;
	//-----------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	TSubclassOf<class ASuraProjectile> LeftProjectileClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	TSubclassOf<class ASuraProjectile> RightProjectileClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	TSubclassOf<class ASuraProjectile> SkillProjectileClass;
	//-----------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	UNiagaraSystem* FireEffect_L = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	UNiagaraSystem* FireEffect_R = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	UNiagaraSystem* FireEffect_Skill = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	UNiagaraSystem* ChargeEffect = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	FVector ChargeEffectLocation = FVector();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	FRotator ChargeEffectRotation = FRotator();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	FVector ChargeEffenctScale = { 1.f, 1.f, 1.f };
	//-----------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* FireSound_L = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* FireSound_R = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* FireSound_Skill = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* ChargeSound = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* TargetSearchLoopSound = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* TargetLockedSound = nullptr;
	//-----------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "IK")
	FTransform RightHandOffset = FTransform();
	//-----------------------------------------------------------------

	/** Start Suhyeon  **/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* WeaponImage = nullptr; // 총기 이미지

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsWeaponOwned = false; // 총기 소유 불값
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AWeapon> WeaponClass; // weaponinventory 동기화 위해서
	
	/** End Suhyeon **/

	//-----------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reload")
	float ReloadingTime = 2.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reload")
	float PumpReloadingTime_Start = 0.6f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reload")
	float PumpReloadingTime_Loop = 0.4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reload")
	float PumpReloadingTime_End = 0.6f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reload")
	float PumpReloadingTime_LoopToFire = 0.2f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reload")
	int32 MaxAmmoPerMag = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reload")
	int32 MaxTotalAmmo = 200.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reload")
	int32 AmmoConsumedPerShot_Left = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reload")
	int32 AmmoConsumedPerShot_Right = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reload")
	int32 AmmoCost_Skill = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reload")
	bool bAllowFireWithInsufficientAmmo_L = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reload")
	bool bAllowFireWithInsufficientAmmo_R = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reload")
	bool bAllowFireWithInsufficientAmmo_Skill = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Reload")
	bool bActivePumpActionReload = false;
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
	bool bEnableMultiProjectile_Left = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MultiProjectile")
	bool bEnableMultiProjectile_Right = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MultiProjectile")
	float MaxAngleOfMultiProjectileSpread_L = 3.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MultiProjectile")
	float MaxAngleOfMultiProjectileSpread_R = 3.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MultiProjectile")
	int32 PelletsNum_L = 6;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MultiProjectile")
	int32 PelletsNum_R = 6;
	//-----------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	FWeaponRecoilStruct DefaultRecoil; //TODO: 제거
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	FWeaponRecoilStruct DefaultRecoil_L;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	FWeaponRecoilStruct DefaultRecoil_R;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	FWeaponRecoilStruct DefaultRecoil_Skill;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	FWeaponRecoilStruct ZoomRecoil;
	//-----------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ArmRecoil")
	FArmRecoilStruct ArmRecoil_L;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ArmRecoil")
	FArmRecoilStruct ArmRecoil_R;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ArmRecoil")
	FArmRecoilStruct ArmRecoil_Skill;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ArmRecoil")
	FArmRecoilStruct ArmRecoil_Hand;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ArmRecoil")
	FArmRecoilStruct ArmRecoil_UpperArm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ArmRecoil")
	FArmRecoilStruct ArmRecoil_LowerArm;
	//-----------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	FTransform RightHandSocketTransform = FTransform();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	FTransform RightHandSocketTransform_Crouch = FTransform();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	FTransform RightHandSocketTransform_Targeting = FTransform();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	FTransform RightHandSocketTransform_Targeting_Crouch = FTransform();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	FTransform SkillWeaponSocketTransform_Active = FTransform();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	FTransform SkillWeaponSocketTransform_Inactive = FTransform();
	//-----------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = "CameraShake")
	TSubclassOf<UWeaponCameraShakeBase> DefaultCameraShakeClass_L;
	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = "CameraShake")
	TSubclassOf<UWeaponCameraShakeBase> DefaultCameraShakeClass_R;
	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = "CameraShake")
	TSubclassOf<UWeaponCameraShakeBase> DefaultCameraShakeClass_Skill;
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	float TargetingGlobalTimeScale = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	float TargetingGlobalTimeDilationSpeed = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	float TargetingSkillCoolDown = 3.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	float MaxTargetingTime = 10.f;
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Charging")
	int32 ChargingAdditionalPelletMaxNum = 0;
	//-----------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Penetration")
	int32 MaxPenetrableObjectsNum_Left = 4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Penetration")
	int32 MaxPenetrableObjectsNum_Right = 4;
	//-----------------------------------------------------------------
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitScan")
	bool bIsHitScan_Left = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitScan")
	bool bIsHitScan_Right = false;
};
