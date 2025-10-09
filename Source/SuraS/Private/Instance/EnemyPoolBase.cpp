// Fill out your copyright notice in the Description page of Project Settings.


#include "Instance/EnemyPoolBase.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Characters/Enemies/AI/EnemyBaseAIController.h"


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
			//AActor* newPoolableActor = UAIBlueprintHelperLibrary::SpawnAIFromClass(World,
			//	*CastedEnemyClass, BehaviorTree, FVector().ZeroVector, FRotator().ZeroRotator, true );
			APawn* NewPoolableActor = GetWorld()->SpawnActor<ASuraCharacterEnemyBase>(PooledObjectSubclass, FVector().ZeroVector, FRotator().ZeroRotator, ActorSpawnParameters);

			Cast<ASuraCharacterEnemyBase>(NewPoolableActor)->InitializeEnemy();
			NewPoolableActor->SetActorHiddenInGame(false);
			ObjectPool.Add(NewPoolableActor);
			spawnedObject = NewPoolableActor;
			return;
		}
	}
}

UEnemyPoolBase::UEnemyPoolBase()
{

}

void UEnemyPoolBase::Initialize(UWorld* const world, int initialAmount, TSubclassOf<AActor> object)
{
	Super::Initialize(world, initialAmount, object);
	//BehaviorTree = Cast<ASuraCharacterEnemyBase>(PooledObject)->GetBehaviorTree();
}
