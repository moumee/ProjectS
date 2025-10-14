// Fill out your copyright notice in the Description page of Project Settings.


#include "Instance/ObjectPoolManager.h"
#include "Characters/Enemies/SuraCharacterEnemyBase.h"
#include "Instance/EnemyPoolBase.h"

UObjectPoolManager::UObjectPoolManager()
{
}

UObjectPoolBase* UObjectPoolManager::GetPool(TSubclassOf<AActor> ObjectClass, UObject* WorldContext)
{
	if (!ObjectClass || !WorldContext) return nullptr;

	if (ObjectPool_List.Contains(ObjectClass)) 
	{
		return *ObjectPool_List.Find(ObjectClass);
	}
	if (ObjectClass->IsChildOf(ASuraCharacterEnemyBase::StaticClass()))
	{
		UE_LOG(LogTemp, Error, TEXT("enemypool"));
		UObjectPoolBase* newPool = NewObject<UEnemyPoolBase>(this, TEXT("ObjectPoolBase"));
		newPool->Initialize(WorldContext->GetWorld(), 10, ObjectClass);
		ObjectPool_List.Add(ObjectClass, newPool);
		return newPool;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("objectpool"));
		UObjectPoolBase* newPool = NewObject<UObjectPoolBase>(this, TEXT("ObjectPoolBase"));
		newPool->Initialize(WorldContext->GetWorld(), 3, ObjectClass);
		ObjectPool_List.Add(ObjectClass, newPool);
		return newPool;
	}


	
}

void UObjectPoolManager::ReturnToPool(AActor* Object)
{
}
