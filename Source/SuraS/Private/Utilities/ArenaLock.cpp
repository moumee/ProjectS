// Fill out your copyright notice in the Description page of Project Settings.


#include "Utilities/ArenaLock.h"

#include "ActorComponents/DamageComponent/ACEnemyDamageSystem.h"
#include "Characters/Enemies/SuraCharacterEnemyBase.h"
#include "Components/BoxComponent.h"

AArenaLock::AArenaLock()
{
	ArenaBox = CreateDefaultSubobject<UBoxComponent>(FName("ArenaBox"));
	RootComponent = ArenaBox;
}

void AArenaLock::BeginPlay()
{
	Super::BeginPlay();

	if (!ArenaEnemies.IsEmpty())
	{
		for (ASuraCharacterEnemyBase* Enemy : ArenaEnemies)
		{
			Enemy->GetDamageSystemComp()->OnDeath.AddWeakLambda(this, [this, Enemy]()
			{
				this->OnEnemyDeath(Enemy);
			});
		}
	}
}

void AArenaLock::OnEnemyDeath(ASuraCharacterEnemyBase* Enemy)
{
	if (Enemy)
	{
		ArenaEnemies.Remove(Enemy);

		if (ArenaEnemies.IsEmpty())
			Destroy();
	}
}