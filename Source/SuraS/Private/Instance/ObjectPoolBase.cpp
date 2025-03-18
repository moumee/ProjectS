//***********************************************************
// 1. 기본 오브젝트 풀 베이스 - Actor 기반
// 2. 월드에 존재x, objectpoolManager에 관리되고 저장
// 3. 자체가 월드를 불러올 수 없으므로 월드를 불러오는 매개변수 필요
// 4. 총알과 적의 소환방식과 후처리가 다르지만, 이동과 겹침 오류처리는 동일하므로 밖에서 사용되는 메소드(GetPooledObject)는 월드와 위치, 회전을 제공받음
// 5.GetPooledObject 내부에서 실제 소환처리를 하는 별개의 함수를 만들고, 상속 클래스에서 원하는 대로 변경
//

#include "Instance/ObjectPoolBase.h"

UObjectPoolBase::UObjectPoolBase()
{
}

void UObjectPoolBase::Initialize(UWorld* const world, int initialAmount, TSubclassOf<class AActor> object)
{
	if (PooledObjectSubclass != nullptr || world != nullptr)
	{
		PooledObjectSubclass = object;
		World = world;
		ActorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ObjectPool = TArray<AActor*>();
		//UE_LOG(LogTemp, Log, TEXT("%b"), World);

		if (World != nullptr)
		{
			for (int i = 0; i < initialAmount; i++)
			{
				AActor* PoolableActor = GetPooledObject(FVector(0, 0, 0), FRotator(0, 0, 0));

				if (PoolableActor != nullptr)
				{
					PoolableActor->SetActorHiddenInGame(true);
					PoolableActor->SetActorEnableCollision(false);
					ObjectPool.Add(PoolableActor);
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("World is nullptr!"));
		}
	}
	else 
	{
		UE_LOG(LogTemp, Warning, TEXT("PooledObjectSubclass is nullptr!"));
	}
}

UObjectPoolBase::~UObjectPoolBase()
{
}

AActor* UObjectPoolBase::GetPooledObject(FVector position, FRotator rotation)
{
	if (true)
	{
		//UE_LOG(LogTemp, Log, TEXT("2"));
		for (AActor* PoolableActor : ObjectPool)
		{
			if (PoolableActor != nullptr && PoolableActor->IsHidden())
			{
				PoolableActor->TeleportTo(position, rotation);
				//PoolableActor->InitializeEnemy();
				PoolableActor->SetActorHiddenInGame(false);
				PoolableActor->SetActorEnableCollision(true);
				return PoolableActor;
			}
		}
	}

	AActor* SpawnedObject;
	SpawnedObject = World->SpawnActor<AActor>(PooledObjectSubclass,
		FVector().ZeroVector, FRotator().ZeroRotator, ActorSpawnParameters);
	ObjectPool.Add(SpawnedObject);
	//SpawnPooledObject(SpawnedObject);
	SpawnedObject->TeleportTo(position, rotation);
	//PoolableActor->InitializeEnemy();
	SpawnedObject->SetActorHiddenInGame(false);
	SpawnedObject->SetActorEnableCollision(true);

	return SpawnedObject;
}

void UObjectPoolBase::SpawnPooledObject(AActor*& spawnedObject)
{
	spawnedObject = World->SpawnActor<AActor>(PooledObjectSubclass,
		FVector().ZeroVector, FRotator().ZeroRotator, ActorSpawnParameters);
	spawnedObject->SetActorHiddenInGame(false);
	ObjectPool.Add(spawnedObject);
}