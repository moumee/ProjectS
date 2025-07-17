// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ActorComponents/WeaponSystem/WeaponStateType.h"
#include "SuraWeaponAnimInstance.generated.h"

class AWeapon;
/**
 * 
 */
UCLASS()
class SURAS_API USuraWeaponAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	AWeapon* Weapon;
	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	EWeaponStateType PreviousWeaponStateType;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	EWeaponStateType CurrentWeaponStateType;

	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	UAnimMontage* AM_Mag_Reload;
};
