// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Navigation/NavLinkProxy.h"
#include "EnemyClimbNavLink.generated.h"

class UBoxComponent;
class ASuraCharacterEnemyBase;
class UNavModifierComponent;

UCLASS()
class SURAS_API AEnemyClimbNavLink : public ANavLinkProxy
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnReceiveSmartLinkReached(AActor* Agent, const FVector& Destination);

public:
	// Sets default values for this actor's properties
	AEnemyClimbNavLink();
};
