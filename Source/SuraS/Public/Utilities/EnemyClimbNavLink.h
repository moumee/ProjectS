// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Navigation/NavLinkProxy.h"
#include "EnemyClimbNavLink.generated.h"

class USphereComponent;
class ASuraCharacterEnemyBase;
class UNavModifierComponent;

UCLASS()
class SURAS_API AEnemyClimbNavLink : public ANavLinkProxy
{
	GENERATED_BODY()

	TWeakObjectPtr<ASuraCharacterEnemyBase> OccupyingEnemy;

	bool bIsOccupied = false;

	UFUNCTION()
	void SetLinkUsable(bool bIsUsable);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnReceiveSmartLinkReached(AActor* Agent, const FVector& Destination);

public:
	// Sets default values for this actor's properties
	AEnemyClimbNavLink();

	UPROPERTY(VisibleAnywhere, BlueprintReadwRITE, Category = "Collision")
	USphereComponent* EndSphere;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
