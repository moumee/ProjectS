// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyLevelSequenceTriggerBox.generated.h"

class ULevelSequencePlayer;
class ULevelSequence;
class UBoxComponent;

UCLASS()
class SURAS_API AEnemyLevelSequenceTriggerBox : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnemyLevelSequenceTriggerBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Trigger LS")
	UBoxComponent* TriggerBox = nullptr;

	UPROPERTY(EditAnywhere, Category = "Level Sequence")
	ULevelSequence* LevelSequenceAsset = nullptr;

	UPROPERTY()
	ULevelSequencePlayer* LevelSequencePlayer = nullptr;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
