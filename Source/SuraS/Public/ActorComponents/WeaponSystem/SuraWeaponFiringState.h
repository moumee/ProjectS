

#pragma once

#include "CoreMinimal.h"
#include "SuraWeaponBaseState.h"
#include "SuraWeaponFiringState.generated.h"

/**
 * 
 */
UCLASS()
class SURAS_API USuraWeaponFiringState : public USuraWeaponBaseState
{
	GENERATED_BODY()
public:
	USuraWeaponFiringState();
	~USuraWeaponFiringState();

	virtual void EnterState(AWeapon* Weapon) override;

	virtual void UpdateState(AWeapon* Weapon, float DeltaTime) override;

	virtual void ExitState(AWeapon* Weapon) override;
};