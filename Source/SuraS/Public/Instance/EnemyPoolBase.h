// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Instance/ObjectPoolBase.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Characters/Enemies/SuraCharacterEnemyBase.h"
#include "EnemyPoolBase.generated.h"

/**
 * 
 */
UCLASS()
class SURAS_API UEnemyPoolBase : public UObjectPoolBase
{
	GENERATED_BODY()
	
protected:
	virtual void SpawnPooledObject(AActor*& spawnedObject) override;
	UBehaviorTree* BehaviorTree;

public:
	UEnemyPoolBase();
	virtual void Initialize(UWorld* const world, int initialAmount, AActor* object) override;
	

};
