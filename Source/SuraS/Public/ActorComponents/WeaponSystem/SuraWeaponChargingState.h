// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SuraWeaponBaseState.h"
#include "SuraWeaponChargingState.generated.h"

/**
 * 
 */
UCLASS()
class SURAS_API USuraWeaponChargingState : public USuraWeaponBaseState
{
	GENERATED_BODY()
public:
	USuraWeaponChargingState();
	~USuraWeaponChargingState();

	virtual void EnterState(AWeapon* Weapon) override;

	virtual void UpdateState(AWeapon* Weapon, float DeltaTime) override;

	virtual void ExitState(AWeapon* Weapon) override;
};
