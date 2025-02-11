// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/WeaponSystem/SuraWeaponReloadingState.h"

USuraWeaponReloadingState::USuraWeaponReloadingState()
{
	WeaponStateType = EWeaponStateType::WeaponStateType_Reloading;
}

USuraWeaponReloadingState::~USuraWeaponReloadingState()
{
}

void USuraWeaponReloadingState::EnterState(UACWeapon* Weapon)
{
	Super::EnterState(Weapon);

	//TODO:
	// ReloadingTime(Ȥ�� ReloadingRate)������ ���� Reloading Anim Montage�� Rate�� �����ϰ� �����ϵ���
	// Reloading�� ������ IdleState�� Change�ϱ�
	// �ٵ� �̰� ���� State���� ������ �ʿ䰡 �ֳ�? �׳� weapon���� ó���ϴ°� ����� �� ������
}

void USuraWeaponReloadingState::UpdateState(UACWeapon* Weapon, float DeltaTime)
{
	Super::UpdateState(Weapon, DeltaTime);
}

void USuraWeaponReloadingState::ExitState(UACWeapon* Weapon)
{
	Super::ExitState(Weapon);
}
