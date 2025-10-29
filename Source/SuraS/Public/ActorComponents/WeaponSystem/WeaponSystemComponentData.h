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
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	//TSubclassOf<class AWeapon> StartingWeaponClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rendering")
	bool bUseSceneCapture = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TMap<EWeaponName, bool> WeaponOwnerShipMap;

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
