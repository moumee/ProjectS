// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponents/WeaponSystem/ACWeapon.h"
#include "ActorComponents/WeaponSystem/ProjectileType.h"
#include "WeaponName.h"
#include "WeaponSystemComponentData.generated.h"

USTRUCT(BlueprintType)
struct SURAS_API FWeaponSystemComponentData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	EWeaponName StartingWeaponName = EWeaponName::WeaponName_Rifle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<class AWeapon> StartingWeaponClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rendering")
	bool bUseSceneCapture = false;
	
	//-----------------------
	//TODO: 각 무기별 소유 여부 판단하는 변수 필요
	// Map Enum 혹은 FName 과 Bool 매핑
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TMap<EWeaponName, bool> WeaponOwnerShipMap;
		
	//TODO: Set TMap<EWeaponName, WeaponClass>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TMap<EWeaponName, TSubclassOf<AWeapon>> WeaponClasses;

	FWeaponSystemComponentData()
	{
		WeaponOwnerShipMap.Add(EWeaponName::WeaponName_Rifle, false);
		WeaponOwnerShipMap.Add(EWeaponName::WeaponName_ShotGun, false);
		WeaponOwnerShipMap.Add(EWeaponName::WeaponName_MissileLauncher, false);
		WeaponOwnerShipMap.Add(EWeaponName::WeaponName_RailGun, false);

		WeaponClasses.Add(EWeaponName::WeaponName_Rifle, AWeapon::StaticClass());
		WeaponClasses.Add(EWeaponName::WeaponName_ShotGun, AWeapon::StaticClass());
		WeaponClasses.Add(EWeaponName::WeaponName_MissileLauncher, AWeapon::StaticClass());
		WeaponClasses.Add(EWeaponName::WeaponName_RailGun, AWeapon::StaticClass());
	}
};
