// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyPatrolRoute.generated.h"

class USplineComponent;

UCLASS()
class SURAS_API AEnemyPatrolRoute : public AActor
{
	GENERATED_BODY()

	UPROPERTY()
	USplineComponent* PatrolRoute;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Patrol Route", meta=(AllowPrivateAccess = "true"))
	bool bIsRouteLooped = false;

	int _PatrolIndex = 0;
	int _Direction = 1;

public:
	// Sets default values for this actor's properties
	AEnemyPatrolRoute();

protected:


public:
	void IncrementPatrolRoute();
	FVector GetCurrentPatrolIndexPosition() const;
};
