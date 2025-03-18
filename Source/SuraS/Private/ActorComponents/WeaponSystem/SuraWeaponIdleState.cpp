
#include "ActorComponents/WeaponSystem/SuraWeaponIdleState.h"
#include "ActorComponents/WeaponSystem/ACWeapon.h"

USuraWeaponIdleState::USuraWeaponIdleState()
{
	WeaponStateType = EWeaponStateType::WeaponStateType_Idle;
}

USuraWeaponIdleState::~USuraWeaponIdleState()
{
}

void USuraWeaponIdleState::EnterState(AWeapon* Weapon)
{
	Super::EnterState(Weapon);
	Weapon->AutoReload();
}

void USuraWeaponIdleState::UpdateState(AWeapon* Weapon, float DeltaTime)
{
	Super::UpdateState(Weapon, DeltaTime);
}

void USuraWeaponIdleState::ExitState(AWeapon* Weapon)
{
	Super::ExitState(Weapon);
}

