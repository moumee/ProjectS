// Fill out your copyright notice in the Description page of Project Settings.


#include "Utilities/AugmentedEnemySpawner.h"

#include "NavigationSystem.h"
#include "ActorComponents/DamageComponent/ACEnemyDamageSystem.h"
#include "Characters/Enemies/SuraCharacterEnemyBase.h"
#include "Characters/Enemies/SuraCharacterEnemyMelee.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Characters/Enemies/AI/EnemyBaseAIController.h"

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

	ASuraPawnPlayer* Player = Cast<ASuraPawnPlayer>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (NumOfEnemiesToWatch <= 0)
	{
		EnemiesToWatch.Empty();

		for (auto [EnemyTypeToSpawn, EnemyBT, NumOfEnemiesToSpawn] : EnemiesToSpawn)
		{
			for (int32 i = 0; i < NumOfEnemiesToSpawn; i++) 
			{
				APawn* SpawnedPawn = UAIBlueprintHelperLibrary::SpawnAIFromClass(GetWorld(), EnemyTypeToSpawn, EnemyBT, GetRandomLocation(), FRotator(0, 0, 0), true);

				if (ASuraCharacterEnemyBase* SpawnedEnemy = Cast<ASuraCharacterEnemyBase>(SpawnedPawn))
				{
					SpawnedEnemy->GetAIController()->SetStateToChaseOrPursue(Player);
				}
			}
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
