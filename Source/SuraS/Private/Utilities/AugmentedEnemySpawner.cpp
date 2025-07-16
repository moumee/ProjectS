// Fill out your copyright notice in the Description page of Project Settings.


#include "Utilities/AugmentedEnemySpawner.h"

#include "NavigationSystem.h"
#include "ActorComponents/DamageComponent/ACEnemyDamageSystem.h"
#include "Characters/Enemies/SuraCharacterEnemyBase.h"
#include "Characters/Enemies/SuraCharacterEnemyMelee.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

void AAugmentedEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	if (EnemiesToWatch.Num() > 0)
	{
		NumOfEnemiesToWatch = EnemiesToWatch.Num();
		
		for (auto Enemy : EnemiesToWatch)
			Enemy->GetDamageSystemComp()->OnDeath.AddUObject(this, &AAugmentedEnemySpawner::OnDeathTriggered);
	}
}

void AAugmentedEnemySpawner::OnDeathTriggered()
{
	NumOfEnemiesToWatch--;

	if (NumOfEnemiesToWatch <= 0)
	{
		EnemiesToWatch.Empty();

		for (FSpawnEnemyInfo EnemyToSpawn : EnemiesToSpawn)
		{
			for (int32 i = 0; i < EnemyToSpawn.NumOfEnemiesToSpawn; i++)
				UAIBlueprintHelperLibrary::SpawnAIFromClass(GetWorld(), EnemyToSpawn.EnemyTypeToSpawn, EnemyToSpawn.EnemyBT, GetRandomLocation(), FRotator(0, 0, 0), true);
		}
	}
}

FVector AAugmentedEnemySpawner::GetRandomLocation()
{
	if (UNavigationSystemV1* const Navigation = UNavigationSystemV1::GetCurrent(GetWorld()))
	{
		FNavLocation RandomLocation;

		if (Navigation->GetRandomPointInNavigableRadius(GetActorLocation(), 100.f, RandomLocation))
			return RandomLocation;
	}

	return GetActorLocation();
}
