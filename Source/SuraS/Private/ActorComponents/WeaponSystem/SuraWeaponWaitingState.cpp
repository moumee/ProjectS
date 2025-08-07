// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/WeaponSystem/SuraWeaponWaitingState.h"
#include "ActorComponents/WeaponSystem/ACWeapon.h"

USuraWeaponWaitingState::USuraWeaponWaitingState()
{
}

USuraWeaponWaitingState::~USuraWeaponWaitingState()
{
}

void USuraWeaponWaitingState::EnterState(AWeapon* Weapon)
{
	Super::EnterState(Weapon);
	WeaponStateType = EWeaponStateType::WeaponStateType_Waiting;
}

void USuraWeaponWaitingState::UpdateState(AWeapon* Weapon, float DeltaTime)
{
	Super::UpdateState(Weapon, DeltaTime);
}

void USuraWeaponWaitingState::ExitState(AWeapon* Weapon)
{
	Super::ExitState(Weapon);
}
