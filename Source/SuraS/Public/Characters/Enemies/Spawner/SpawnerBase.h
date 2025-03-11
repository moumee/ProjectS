// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Instance/ObjectPoolBase.h"
#include "SpawnerBase.generated.h"

UCLASS()
class SURAS_API ASpawnerBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ASpawnerBase();

	void SpawnWrapper();

	UPROPERTY(EditAnywhere, Category = "ObjectPool")
	TSubclassOf<class AActor> PooledObjectSubclass;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "ObjectPool")
	float interval = 20.0f;
	UPROPERTY(EditAnywhere, Category = "ObjectPool")
	int spawnCount = 5;

	FTimerHandle TimerHandler;
	UObjectPoolBase* Pool;

};
