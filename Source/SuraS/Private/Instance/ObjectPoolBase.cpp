//***********************************************************
// 1. �⺻ ������Ʈ Ǯ ���̽� - Actor ���
// 2. ���忡 ����x, objectpoolManager�� �����ǰ� ����
// 3. ��ü�� ���带 �ҷ��� �� �����Ƿ� ���带 �ҷ����� �Ű����� �ʿ�
// 4. �Ѿ˰� ���� ��ȯ��İ� ��ó���� �ٸ�����, �̵��� ��ħ ����ó���� �����ϹǷ� �ۿ��� ���Ǵ� �޼ҵ�(GetPooledObject)�� ����� ��ġ, ȸ���� ��������
// 5.GetPooledObject ���ο��� ���� ��ȯó���� �ϴ� ������ �Լ��� �����, ��� Ŭ�������� ���ϴ� ��� ����
//

#include "Instance/ObjectPoolBase.h"

UObjectPoolBase::UObjectPoolBase()
{
}

void UObjectPoolBase::Initialize(UWorld* const world, int initialAmount, TSubclassOf<AActor> object)
{
	if (object != nullptr || world != nullptr)
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
		UE_LOG(LogTemp, Log, TEXT("2"));
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
	SpawnPooledObject(SpawnedObject);
	SpawnedObject = World->SpawnActor<AActor>(PooledObjectSubclass,
		FVector().ZeroVector, FRotator().ZeroRotator, ActorSpawnParameters);
	 if (SpawnedObject)
	 {
	 	ObjectPool.Add(SpawnedObject);
	
	 	SpawnedObject->TeleportTo(position, rotation);
	 	//PoolableActor->InitializeEnemy();
	 	SpawnedObject->SetActorHiddenInGame(false);
	 	SpawnedObject->SetActorEnableCollision(true);
	 	return SpawnedObject;
	 }
	
	return nullptr;
}

void UObjectPoolBase::SpawnPooledObject(AActor*& spawnedObject)
{
	spawnedObject = World->SpawnActor<AActor>(PooledObjectSubclass,
		FVector().ZeroVector, FRotator().ZeroRotator, ActorSpawnParameters);
	spawnedObject->SetActorHiddenInGame(false);
	ObjectPool.Add(spawnedObject);
}