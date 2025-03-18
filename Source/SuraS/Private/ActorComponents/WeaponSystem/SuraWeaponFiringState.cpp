


#include "ActorComponents/WeaponSystem/SuraWeaponFiringState.h"
//#include "ActorComponents/WeaponSystem/ACWeapon.h"

USuraWeaponFiringState::USuraWeaponFiringState()
{
	WeaponStateType = EWeaponStateType::WeaponStateType_Firing;
}

USuraWeaponFiringState::~USuraWeaponFiringState()
{
}

void USuraWeaponFiringState::EnterState(AWeapon* Weapon)
{
	Super::EnterState(Weapon);
}

void USuraWeaponFiringState::UpdateState(AWeapon* Weapon, float DeltaTime)
{
	Super::UpdateState(Weapon, DeltaTime);
}

void USuraWeaponFiringState::ExitState(AWeapon* Weapon)
{
	Super::ExitState(Weapon);
	//Weapon->AutoReload();
}