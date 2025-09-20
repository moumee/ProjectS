// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "SuraCheckpoint.generated.h"

class UCapsuleComponent;

UCLASS()
class SURAS_API ASuraCheckpoint : public ATriggerBox
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASuraCheckpoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnTriggerBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UPROPERTY(EditAnywhere)
	TObjectPtr<UCapsuleComponent> SpawnCapsule;

	UPROPERTY(EditAnywhere)
	int32 CheckpointOrderIndex;
};
