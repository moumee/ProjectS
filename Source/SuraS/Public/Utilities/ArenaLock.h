// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "ArenaLock.generated.h"

class ASuraCharacterEnemyBase;

UCLASS()
class SURAS_API AArenaLock : public AActor
{
	GENERATED_BODY()

	void OnEnemyDeath(ASuraCharacterEnemyBase* Enemy);

public:
	AArenaLock();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void UpdateDissolveColor(float Alpha);

	UPROPERTY(VisibleDefaultsOnly)
	TObjectPtr<UTimelineComponent> DissolveTimeline;
	FOnTimelineFloat OnDissolveColorTimelineFloat;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ArenaLock")
	TArray<ASuraCharacterEnemyBase*> ArenaEnemies;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ArenaLock")
	TObjectPtr<UStaticMeshComponent> ArenaMesh;

	UPROPERTY(EditDefaultsOnly, Category = "ArenaLock")
	TObjectPtr<UCurveFloat> DissolveColorCurve;
};
