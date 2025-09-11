// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Navigation/NavLinkProxy.h"
#include "EnemyTraverseLink.generated.h"

/**
 * 
 */
UCLASS()
class SURAS_API AEnemyTraverseLink : public ANavLinkProxy
{
	GENERATED_BODY()

	UPROPERTY()
	ASuraCharacterEnemyBase* CachedEnemy;

	void OnCachedEnemyDeath();

	TArray<FVector> WorldPathPoints;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnReceiveSmartLinkReached(AActor* Agent, const FVector& Destination);

	virtual void OnConstruction(const FTransform& Transform) override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Navigation", Meta = (MakeEditWidget = true))
	TArray<FVector> PathPoints;

	UPROPERTY(EditAnywhere, Category = "NavLink Automation")
	AActor* TargetSimpleLinkActor;
	
	AEnemyTraverseLink();

	void SetLinkUsable(bool bIsUsable);

	UFUNCTION()
	ASuraCharacterEnemyBase* GetCachedEnemy() const { return CachedEnemy; };
};
