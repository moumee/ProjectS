// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h" 
#include "ObjectPoolBase.generated.h" 

/**
 * 
 */
UCLASS()
class SURAS_API UObjectPoolBase : public UObject
{
	GENERATED_BODY()

public:
	UObjectPoolBase();
	~UObjectPoolBase();

	virtual void Initialize(UWorld* const world, int initialAmount, AActor* object);

	UFUNCTION(BlueprintCallable, Category = "ObjectPool")
	AActor* GetPooledObject(FVector position, FRotator rotation);

	UPROPERTY(EditAnywhere, Category = "ObjectPool")
	AActor* PooledObject;
	TSubclassOf<class AActor> PooledObjectSubclass;

protected:

	virtual void SpawnPooledObject(AActor*& spawnedObject);

	TArray<AActor*> ObjectPool;
	FActorSpawnParameters ActorSpawnParameters;
	UWorld* World;

private:

	

	
};
