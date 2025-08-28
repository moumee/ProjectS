// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/DamageComponent/ACBossDamageSystem.h"

#include "Structures/DamageData.h"

bool UACBossDamageSystem::TakeDamage(const FDamageData& DamageData, AActor* DamageCauser)
{
	return Super::TakeDamage(DamageData, DamageCauser);
}
