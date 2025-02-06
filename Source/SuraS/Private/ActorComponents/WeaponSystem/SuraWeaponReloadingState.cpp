// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/WeaponSystem/SuraWeaponReloadingState.h"

USuraWeaponReloadingState::USuraWeaponReloadingState()
{
}

USuraWeaponReloadingState::~USuraWeaponReloadingState()
{
}

void USuraWeaponReloadingState::EnterState(UACWeapon* Weapon)
{
	Super::EnterState(Weapon);

	//TODO:
	// ReloadingTime(Ȥ�� ReloadingRate)������ ���� Reloading Anim Montage�� Rate�� �����ϰ� �����ϵ���
}

void USuraWeaponReloadingState::UpdateState(UACWeapon* Weapon, float DeltaTime)
{
	Super::UpdateState(Weapon, DeltaTime);
}

void USuraWeaponReloadingState::ExitState(UACWeapon* Weapon)
{
	Super::ExitState(Weapon);
}
