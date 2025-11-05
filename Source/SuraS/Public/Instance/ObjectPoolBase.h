// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTree.h"
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

	virtual void Initialize(UWorld* const world, int initialAmount, TSubclassOf<AActor> object);

	UFUNCTION(BlueprintCallable, Category = "ObjectPool")
	AActor* GetPooledObject(FVector position, FRotator rotation);

	UPROPERTY(EditAnywhere, Category = "ObjectPool")
	AActor* PooledObject = nullptr;
	TSubclassOf<class AActor> PooledObjectSubclass;

	UPROPERTY()
	UBehaviorTree* BehaviorTree = nullptr;

protected:

	virtual void SpawnPooledObject(AActor*& spawnedObject);

	UPROPERTY()
	TArray<AActor*> ObjectPool;
	FActorSpawnParameters ActorSpawnParameters;
	
	UPROPERTY()
	UWorld* World = nullptr;


private:

	

	
};
