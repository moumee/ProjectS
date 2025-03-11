// Fill out your copyright notice in the Description page of Project Settings.


#include "Instance/ObjectPoolManager.h"

UObjectPoolManager::UObjectPoolManager()
{
}

UObjectPoolBase* UObjectPoolManager::GetPool(UClass* ObjectClass, UObject* WorldContext)
{
	if (!ObjectClass || !WorldContext) return nullptr;

	if (ObjectPool_List.Contains(ObjectClass)) 
	{
		return *ObjectPool_List.Find(ObjectClass);
	}
	else 
	{
		UObjectPoolBase* newPool = NewObject<UObjectPoolBase>(this, UObjectPoolBase::StaticClass(), TEXT("ObjectPoolBase"));
		ObjectPool_List.Add(ObjectClass, newPool);
	}

	return nullptr;
}

void UObjectPoolManager::ReturnToPool(AActor* Object)
{
}
