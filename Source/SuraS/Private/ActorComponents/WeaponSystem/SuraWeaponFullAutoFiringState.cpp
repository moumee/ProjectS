// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/WeaponSystem/SuraWeaponFullAutoFiringState.h"

USuraWeaponFullAutoFiringState::USuraWeaponFullAutoFiringState()
{
	WeaponStateType = EWeaponStateType::WeaponStateType_FullAutoFiring;
}

USuraWeaponFullAutoFiringState::~USuraWeaponFullAutoFiringState()
{
}

void USuraWeaponFullAutoFiringState::EnterState(AWeapon* Weapon)
{
	Super::EnterState(Weapon);
}

void USuraWeaponFullAutoFiringState::UpdateState(AWeapon* Weapon, float DeltaTime)
{
	Super::UpdateState(Weapon, DeltaTime);
}

void USuraWeaponFullAutoFiringState::ExitState(AWeapon* Weapon)
{
	Super::ExitState(Weapon);
}
