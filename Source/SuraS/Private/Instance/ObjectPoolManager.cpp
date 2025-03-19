// Fill out your copyright notice in the Description page of Project Settings.


#include "Instance/ObjectPoolManager.h"
#include "Characters/Enemies/SuraCharacterEnemyBase.h"
#include "Instance/EnemyPoolBase.h"

UObjectPoolManager::UObjectPoolManager()
{
}

UObjectPoolBase* UObjectPoolManager::GetPool(AActor* ObjectClass, UObject* WorldContext)
{
	if (!ObjectClass || !WorldContext) return nullptr;

	//FScopeLock Lock(&MyCriticalSection);

	if (ObjectPool_List.Contains(ObjectClass)) 
	{
		return *ObjectPool_List.Find(ObjectClass);
	}
	else 
	{
		if (true)
		{
			UObjectPoolBase* newPool = NewObject<UEnemyPoolBase>(this, UEnemyPoolBase::StaticClass(), TEXT("ObjectPoolBase"));
			newPool->Initialize(GetWorld(), 3, ObjectClass);
			ObjectPool_List.Add(ObjectClass, newPool);
			return newPool;
		}
		else
		{
			UObjectPoolBase* newPool = NewObject<UObjectPoolBase>(this, UObjectPoolBase::StaticClass(), TEXT("ObjectPoolBase"));
			newPool->Initialize(GetWorld(), 3, ObjectClass);
			ObjectPool_List.Add(ObjectClass, newPool);
			return newPool;
		}
		
	}

	
}

void UObjectPoolManager::ReturnToPool(AActor* Object)
{
}
