// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AugmentedEnemySpawner.generated.h"

class UBehaviorTree;
class ASuraCharacterEnemyBase;

USTRUCT(BlueprintType)
struct FSpawnEnemyInfo
{
	GENERATED_BODY()
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Spawn System")
	TSubclassOf<ASuraCharacterEnemyBase> EnemyTypeToSpawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Spawn System")
	UBehaviorTree* EnemyBT;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Spawn System")
	int32 NumOfEnemiesToSpawn = 0;
};

UCLASS()
class SURAS_API AAugmentedEnemySpawner : public AActor
{
	GENERATED_BODY()

	int32 NumOfEnemiesToWatch = 0;

	void OnDeathTriggered();

	bool FindValidSpawnLocation(TSubclassOf<ASuraCharacterEnemyBase> EnemyClass, FVector& OutLocation, int32 MaxAttempts);

protected:
	virtual void BeginPlay() override;
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Spawn System")
	float SpawnRadius = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Spawn System")
	float MinSpawnDistanceBtwEnemies = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Spawn System")
	int32 MaxSpawnAttempts = 3;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Spawn System")
	TArray<ASuraCharacterEnemyBase*> EnemiesToWatch;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemy Spawn System")
	TArray<FSpawnEnemyInfo> EnemiesToSpawn;
};
