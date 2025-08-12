// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/WeaponSystem/SuraWeaponWaitingState.h"
#include "ActorComponents/WeaponSystem/ACWeapon.h"

USuraWeaponWaitingState::USuraWeaponWaitingState()
{
	WeaponStateType = EWeaponStateType::WeaponStateType_Waiting;
}

USuraWeaponWaitingState::~USuraWeaponWaitingState()
{
}

void USuraWeaponWaitingState::EnterState(AWeapon* Weapon)
{
	Super::EnterState(Weapon);
	Weapon->ActivateAimUIWidget(false);
}

void USuraWeaponWaitingState::UpdateState(AWeapon* Weapon, float DeltaTime)
{
	Super::UpdateState(Weapon, DeltaTime);
}

void USuraWeaponWaitingState::ExitState(AWeapon* Weapon)
{
	Super::ExitState(Weapon);
	Weapon->ActivateAimUIWidget(true);
}
