// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/Spawner/SpawnerBase.h"
#include "Instance/ObjectPoolManager.h"

// Sets default values
ASpawnerBase::ASpawnerBase()
{

}

void ASpawnerBase::SpawnWrapper()
{
	for (int i = 0; i < spawnCount; i++)
	{
		Pool->GetPooledObject(GetActorLocation(), FRotator().ZeroRotator);
	}
}

// Called when the game starts or when spawned
void ASpawnerBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (PooledObjectSubclass) 
	{
		Pool = GetGameInstance()->GetSubsystem<UObjectPoolManager>()->GetPool(PooledObjectSubclass, GetWorld());
		GetWorld()->GetTimerManager().SetTimer(TimerHandler, this, &ASpawnerBase::SpawnWrapper, interval, true);
	}
}


