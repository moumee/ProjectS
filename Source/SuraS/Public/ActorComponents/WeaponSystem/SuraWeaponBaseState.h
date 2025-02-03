

#pragma once

#include "CoreMinimal.h"
#include "SuraWeaponBaseState.generated.h"

class UACWeapon;
/**
 * 
 */
UCLASS(Abstract)
class SURAS_API USuraWeaponBaseState : public UObject
{
	GENERATED_BODY()

public:

	FName StateDisplayName;

	virtual void EnterState(UACWeapon* Weapon);

	virtual void UpdateState(UACWeapon* Weapon, float DeltaTime);

	virtual void ExitState(UACWeapon* Weapon);


	//TODO: Reloading, Equiping, Unequip -> Switching? (��� �� �𸣰ڳ�), OutOfAmmo, ZoomIn ZoomOut
	// IdleState
	// FiringState
	// ReloadingState
	

};
