// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Instance/ObjectPoolBase.h"
#include "AIController.h"
#include "SpawnerBase.generated.h"

UCLASS()
class SURAS_API ASpawnerBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ASpawnerBase();
	virtual void Tick(float DeltaTime) override;

	void SpawnWrapper();

	UPROPERTY(EditAnywhere, Category = "ObjectPool")
	AActor* PooledObject;

	UPROPERTY(EditAnywhere, Category = "ObjectPool")
	UBehaviorTree* BehaviorTree;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "ObjectPool")
	float interval = 5.0f;
	UPROPERTY(EditAnywhere, Category = "ObjectPool")
	int spawnCount = 1;

	FTimerHandle TimerHandler;
	UPROPERTY()
	UObjectPoolBase* Pool;

};
