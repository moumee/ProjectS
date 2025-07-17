// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/WeaponSystem/SuraWeaponAnimInstance.h"

#include "ActorComponents/WeaponSystem/ACWeapon.h"
#include "ActorComponents/WeaponSystem/SuraWeaponBaseState.h"

void USuraWeaponAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	WeaponMesh = GetOwningComponent();

	if (WeaponMesh)
	{
		Weapon = Cast<AWeapon>(WeaponMesh->GetOwner());
	}

	PreviousWeaponStateType = EWeaponStateType::WeaponStateType_None;
	CurrentWeaponStateType = EWeaponStateType::WeaponStateType_None;
}

void USuraWeaponAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	if (Weapon)
	{
		if (Weapon->GetCurrentState())
		{
			if (CurrentWeaponStateType != Weapon->GetCurrentState()->GetWeaponStateType())
			{
				PreviousWeaponStateType = CurrentWeaponStateType;
				CurrentWeaponStateType = Weapon->GetCurrentState()->GetWeaponStateType();

				if (CurrentWeaponStateType == EWeaponStateType::WeaponStateType_Reloading)
				{
					Montage_Play(AM_Mag_Reload);
				}
			}
		}
	}
}
