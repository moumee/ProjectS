// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/DamageComponent/ACEnemyDamageSystem.h"


// Sets default values for this component's properties
UACEnemyDamageSystem::UACEnemyDamageSystem()
{
}

bool UACEnemyDamageSystem::TakeDamage(const FDamageData& DamageData, const AActor* DamageCauser)
{
	// 1. Get hit location and subtract it from the relevant part of the body
	// 2. Calculate the whole health based on the subtract factor on each body
	
	return Super::TakeDamage(DamageData, DamageCauser);
}