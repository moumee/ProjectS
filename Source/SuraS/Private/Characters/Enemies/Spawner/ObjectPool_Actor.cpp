// Fill out your copyright notice in the Description page of Project Settings.
//
// 1. ������Ʈ Ǯ���� ���� ������ �׽�Ʈ
// 2. Actor�� ���������� ��ȯ�ϴ� ���̾�����, initialize�� �ʿ��� �͵��� �־� ASuraCharacterEnemyBase �������� ����
// 3. ��ȯ����� SpawnAIFromClass�� ����, BehaviorTree ����
// 4. ������ �� ��ȯ���� �ʴ� ��츦 ���� �ش� ��츦 �����ϴ� ActorSpawnParameter ����
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


