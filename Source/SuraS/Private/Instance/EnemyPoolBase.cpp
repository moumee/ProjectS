// Fill out your copyright notice in the Description page of Project Settings.


#include "Instance/EnemyPoolBase.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"


void UEnemyPoolBase::SpawnPooledObject(AActor*& spawnedObject)
{
	if (true)
	{
		//UE_LOG(LogTemp, Log, TEXT("2"));
		for (AActor* PoolableActor : ObjectPool)
		{
			if (PoolableActor != nullptr && PoolableActor->IsHidden())
			{
				PoolableActor->TeleportTo(FVector(0, 0, 0), FRotator(0, 0, 0));
				Cast<ASuraCharacterEnemyBase>(PoolableActor)->InitializeEnemy();
				PoolableActor->SetActorHiddenInGame(false);
				spawnedObject = PoolableActor;
				return;
			}
		}
		TSubclassOf<APawn> CastedEnemyClass = TSubclassOf<APawn>(PooledObjectSubclass);
		if (World != nullptr)
		{
			
			AActor* newPoolableActor = UAIBlueprintHelperLibrary::SpawnAIFromClass(World,
				*CastedEnemyClass, BehaviorTree, FVector().ZeroVector, FRotator().ZeroRotator, true);
			Cast<ASuraCharacterEnemyBase>(newPoolableActor)->InitializeEnemy();
			newPoolableActor->SetActorHiddenInGame(false);
			ObjectPool.Add(newPoolableActor);
			spawnedObject = newPoolableActor;
			return;
		}
	}
}

UEnemyPoolBase::UEnemyPoolBase()
{

}

void UEnemyPoolBase::Initialize(UWorld* const world, int initialAmount, AActor* object)
{
	Super::Initialize(world, initialAmount, object);
	BehaviorTree = Cast<ASuraCharacterEnemyBase>(PooledObject)->GetBehaviorTree();
}
