

#pragma once

#include "CoreMinimal.h"
#include "SuraWeaponBaseState.h"
#include "SuraWeaponIdleState.generated.h"

/**
 * 
 */
UCLASS()
class SURAS_API USuraWeaponIdleState : public USuraWeaponBaseState
{
	GENERATED_BODY()
public:
	USuraWeaponIdleState();
	~USuraWeaponIdleState();

	virtual void EnterState(AWeapon* Weapon) override;

	virtual void UpdateState(AWeapon* Weapon, float DeltaTime)override;

	virtual void ExitState(AWeapon* Weapon)override;
};
