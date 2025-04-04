// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Firearms/SuraFirearmBase.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
ASuraFirearmBase::ASuraFirearmBase()
{
}

void ASuraFirearmBase::InitializeFirearem(ASuraCharacterEnemyBase* OwnerChar, int32 NewMaxAmmo, int32 NewMagsize)
{
	Owner = OwnerChar;
	OwnerCharacter = OwnerChar;
	MaxAmmo = NewMaxAmmo;
	CurrentAmmoInReserve = NewMaxAmmo;
	MagSize = NewMagsize;

	Reload();
}

void ASuraFirearmBase::Fire(AActor* FirearmOwner, const AActor* TargetActor)
{
	return;
}

bool ASuraFirearmBase::ShouldReload()
{
	if (CurrentAmmonInMag == 0 && CurrentAmmoInReserve != 0)
		return true;

	return false;
}

bool ASuraFirearmBase::CanReload()
{
	if (CurrentAmmonInMag < MagSize && CurrentAmmoInReserve != 0)
		return true;

	return false;
}

void ASuraFirearmBase::Reload()
{
	int32 TotalAmmo = CurrentAmmoInReserve + CurrentAmmonInMag;

	CurrentAmmonInMag = TotalAmmo % MagSize == 0 ? MagSize : TotalAmmo % MagSize;
	CurrentAmmoInReserve = TotalAmmo - CurrentAmmonInMag;
}