// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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

};
