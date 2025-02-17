// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/WeaponSystem/SuraWeaponUnequippedState.h"

#include "ActorComponents/WeaponSystem/ACWeapon.h"

USuraWeaponUnequippedState::USuraWeaponUnequippedState()
{
	WeaponStateType = EWeaponStateType::WeaponStateType_Unequipped;
}

USuraWeaponUnequippedState::~USuraWeaponUnequippedState()
{
}

void USuraWeaponUnequippedState::EnterState(UACWeapon* Weapon)
{
	Super::EnterState(Weapon);

	//TODO: �����Լ��� ��� �����ϴ� �Լ� �����
	Weapon->ForceStopCamModification();
	Weapon->ActivateCrosshairWidget(false);
	Weapon->ActivateAmmoCounterWidget(false);
}

void USuraWeaponUnequippedState::UpdateState(UACWeapon* Weapon, float DeltaTime)
{
	Super::UpdateState(Weapon, DeltaTime);
}

void USuraWeaponUnequippedState::ExitState(UACWeapon* Weapon)
{
	Super::ExitState(Weapon);
	//Weapon->StartCameraSettingChange(&Weapon->CamSetting_Default); //TODO: �� ó���� �� �ʿ��Ѱ�? ����غ�����
}
