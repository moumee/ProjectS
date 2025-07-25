// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyEndClimbingTrigger.generated.h"

class AEnemyClimbNavLink;
class UBoxComponent;

UCLASS()
class SURAS_API AEnemyEndClimbingTrigger : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnemyEndClimbingTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TriggerEnd")
	UBoxComponent* TriggerBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TriggerEnd")
	TArray<AEnemyClimbNavLink*> ClimbNavLinks;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
