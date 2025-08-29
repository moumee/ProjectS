// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyCoopAttackTriggerBox.generated.h"

class ASuraCharacterEnemyBase;
class UBoxComponent;

UCLASS()
class SURAS_API AEnemyCoopAttackTriggerBox : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnemyCoopAttackTriggerBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TriggerEnemies")
	TArray<ASuraCharacterEnemyBase*> EnemiesForCoopAttack;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "TriggerEnemies")
	UBoxComponent* TriggerBox;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
