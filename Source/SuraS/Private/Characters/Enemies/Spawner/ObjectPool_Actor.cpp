// Fill out your copyright notice in the Description page of Project Settings.
//
// 1. 오브젝트 풀링을 위한 스포너 테스트
// 2. Actor를 공통적으로 소환하는 것이었으나, initialize에 필요한 것들이 있어 ASuraCharacterEnemyBase 기준으로 변경
// 3. 소환방식을 SpawnAIFromClass로 변경, BehaviorTree 적용
// 4. 겹쳤을 때 소환되지 않는 경우를 위해 해당 경우를 무시하는 ActorSpawnParameter 적용
//

#include "Characters/Enemies/Spawner/ObjectPool_Actor.h"

// Sets default values
AObjectPool_Actor::AObjectPool_Actor()
{
	PrimaryActorTick.bCanEverTick = false;

}

//
ASuraCharacterEnemyBase* AObjectPool_Actor::SpawnPooledObject()
{
	for (ASuraCharacterEnemyBase* PoolableActor : ObjectPool)
	{
		if (PoolableActor != nullptr && PoolableActor->IsHidden())
		{
			PoolableActor->TeleportTo(GetActorLocation()+FVector(FMath::RandRange(-50, 50), FMath::RandRange(-50, 50), 0), FRotator(0, 0, 0));
			PoolableActor->InitializeEnemy();
			PoolableActor->SetActorHiddenInGame(false);
			PoolableActor->SetActorEnableCollision(true);
			//FString error = (PoolableActor->IsHidden()) ? "true" : "false";
			//UE_LOG(LogBlueprint, Warning, TEXT("%s"), *error);
			return PoolableActor;
		}
	}

	UWorld* const World = GetWorld();
	if (World != nullptr)
	{
		ASuraCharacterEnemyBase* newPoolableActor = Cast<ASuraCharacterEnemyBase>(UAIBlueprintHelperLibrary::SpawnAIFromClass(World,
			PooledObjectSubclass, BehaviorTree, GetActorLocation() + FVector(FMath::RandRange(-50, 50), FMath::RandRange(-50, 50), 0), FRotator().ZeroRotator, true));
		newPoolableActor->SetActorHiddenInGame(false);
		ObjectPool.Add(newPoolableActor);
		return newPoolableActor;
	}

	return nullptr;
}

void AObjectPool_Actor::SpawnWrapper()
{
	for (int i = 0; i < spawnCount; i++)
	{
		SpawnPooledObject();
	}
	//UE_LOG(LogBlueprint, Warning, TEXT("called"));
}


// Called when the game starts or when spawned
void AObjectPool_Actor::BeginPlay()
{
	Super::BeginPlay();


	if (PooledObjectSubclass != nullptr)
	{
		UWorld* const World = GetWorld();
		ActorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		if (World != nullptr)
		{
			for (int i = 0; i < PoolSize; i++)
			{
				ASuraCharacterEnemyBase* PoolableActor = SpawnPooledObject();

				if (PoolableActor != nullptr)
				{
					PoolableActor->SetActorHiddenInGame(true);
					PoolableActor->SetActorEnableCollision(false);
					ObjectPool.Add(PoolableActor);
				}
			}
		}

	}

	GetWorld()->GetTimerManager().SetTimer(TimerHandler, this, &AObjectPool_Actor::SpawnWrapper, interval, true);
	
}


