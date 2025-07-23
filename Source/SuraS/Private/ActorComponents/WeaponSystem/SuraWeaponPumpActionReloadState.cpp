// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/WeaponSystem/SuraWeaponPumpActionReloadState.h"
#include "ActorComponents/WeaponSystem/ACWeapon.h"

USuraWeaponPumpActionReloadState::USuraWeaponPumpActionReloadState()
{
	WeaponStateType = EWeaponStateType::WeaponStateType_PumpActionReloading;
}

USuraWeaponPumpActionReloadState::~USuraWeaponPumpActionReloadState()
{
}

void USuraWeaponPumpActionReloadState::EnterState(AWeapon* Weapon)
{
	Super::EnterState(Weapon);
	Weapon->StartPumpActionReload();
}

void USuraWeaponPumpActionReloadState::UpdateState(AWeapon* Weapon, float DeltaTime)
{
	Super::UpdateState(Weapon, DeltaTime);
}

void USuraWeaponPumpActionReloadState::ExitState(AWeapon* Weapon)
{
	Super::ExitState(Weapon);
}
