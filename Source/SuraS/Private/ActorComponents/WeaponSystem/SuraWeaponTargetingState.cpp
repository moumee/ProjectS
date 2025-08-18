// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/WeaponSystem/SuraWeaponTargetingState.h"
#include "ActorComponents/WeaponSystem/ACWeapon.h"

USuraWeaponTargetingState::USuraWeaponTargetingState()
{
	WeaponStateType = EWeaponStateType::WeaponStateType_Targeting;
}

USuraWeaponTargetingState::~USuraWeaponTargetingState()
{
}

void USuraWeaponTargetingState::EnterState(AWeapon* Weapon)
{
	Super::EnterState(Weapon);
	Weapon->SetGlobalTimeDilation(Weapon->TargetingGlobalTimeScale);
	if (Weapon->IsSkillWeapon()) { Weapon->ActivateAimUIWidget(true); }
}

void USuraWeaponTargetingState::UpdateState(AWeapon* Weapon, float DeltaTime)
{
	Super::UpdateState(Weapon, DeltaTime);
	Weapon->UpdateTargetMarkers();
}

void USuraWeaponTargetingState::ExitState(AWeapon* Weapon)
{
	Super::ExitState(Weapon);
	Weapon->SetGlobalTimeDilation(1.f);
	if (Weapon->IsSkillWeapon()) { Weapon->ActivateAimUIWidget(false); }
}
