// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/WeaponSystem/SuraWeaponSwitchingState.h"
#include "ActorComponents/WeaponSystem/ACWeapon.h"

USuraWeaponSwitchingState::USuraWeaponSwitchingState()
{
	WeaponStateType = EWeaponStateType::WeaponStateType_Switching;
}

USuraWeaponSwitchingState::~USuraWeaponSwitchingState()
{
}

void USuraWeaponSwitchingState::EnterState(AWeapon* Weapon)
{
	Super::EnterState(Weapon);
	Weapon->ZoomOut();
}

void USuraWeaponSwitchingState::UpdateState(AWeapon* Weapon, float DeltaTime)
{
	Super::UpdateState(Weapon, DeltaTime);
}

void USuraWeaponSwitchingState::ExitState(AWeapon* Weapon)
{
	Super::ExitState(Weapon);
}
