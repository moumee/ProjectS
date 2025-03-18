// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/WeaponSystem/SuraWeaponReloadingState.h"
#include "ActorComponents/WeaponSystem/ACWeapon.h"

USuraWeaponReloadingState::USuraWeaponReloadingState()
{
	WeaponStateType = EWeaponStateType::WeaponStateType_Reloading;
}

USuraWeaponReloadingState::~USuraWeaponReloadingState()
{
}

void USuraWeaponReloadingState::EnterState(AWeapon* Weapon)
{
	Super::EnterState(Weapon);
	Weapon->StartReload();
}

void USuraWeaponReloadingState::UpdateState(AWeapon* Weapon, float DeltaTime)
{
	Super::UpdateState(Weapon, DeltaTime);
}

void USuraWeaponReloadingState::ExitState(AWeapon* Weapon)
{
	Super::ExitState(Weapon);
}
