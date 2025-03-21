// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/WeaponSystem/SuraWeaponChargingState.h"

USuraWeaponChargingState::USuraWeaponChargingState()
{
	WeaponStateType = EWeaponStateType::WeaponStateType_Charging;
}

USuraWeaponChargingState::~USuraWeaponChargingState()
{
}

void USuraWeaponChargingState::EnterState(AWeapon* Weapon)
{
	Super::EnterState(Weapon);
}

void USuraWeaponChargingState::UpdateState(AWeapon* Weapon, float DeltaTime)
{
	Super::UpdateState(Weapon, DeltaTime);
}

void USuraWeaponChargingState::ExitState(AWeapon* Weapon)
{
	Super::ExitState(Weapon);
}
