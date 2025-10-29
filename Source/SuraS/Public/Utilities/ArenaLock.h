// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArenaLock.generated.h"

class ASuraCharacterEnemyBase;
class UBoxComponent;

UCLASS()
class SURAS_API AArenaLock : public AActor
{
	GENERATED_BODY()

	void OnEnemyDeath(ASuraCharacterEnemyBase* Enemy);

public:
	AArenaLock();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TriggerEnemies")
	TArray<ASuraCharacterEnemyBase*> ArenaEnemies;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TriggerEnemies")
	UBoxComponent* ArenaBox;
};
