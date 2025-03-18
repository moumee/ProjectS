


#include "ActorComponents/WeaponSystem/SuraWeaponBaseState.h"

#include "ActorComponents/WeaponSystem/ACWeapon.h"

EWeaponStateType USuraWeaponBaseState::GetWeaponStateType() const
{
    return WeaponStateType;
}

void USuraWeaponBaseState::EnterState(AWeapon* Weapon)
{
}

void USuraWeaponBaseState::UpdateState(AWeapon* Weapon, float DeltaTime)
{
}

void USuraWeaponBaseState::ExitState(AWeapon* Weapon)
{
}
