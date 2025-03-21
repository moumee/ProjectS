

#pragma once

#include "CoreMinimal.h"
#include "WeaponStateType.h"
#include "SuraWeaponBaseState.generated.h"

class AWeapon;
/**
 * 
 */
UCLASS(Abstract)
class SURAS_API USuraWeaponBaseState : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, Category = "WeaponState")
	EWeaponStateType WeaponStateType;

	FName StateDisplayName;

	virtual EWeaponStateType GetWeaponStateType() const;

	virtual void EnterState(AWeapon* Weapon);

	virtual void UpdateState(AWeapon* Weapon, float DeltaTime);

	virtual void ExitState(AWeapon* Weapon);


	//TODO: Reloading, Equiping, Unequip -> Switching? (얘는 잘 모르겠네), OutOfAmmo, ZoomIn ZoomOut
	// IdleState
	// FiringState
	// ReloadingState
	

};
