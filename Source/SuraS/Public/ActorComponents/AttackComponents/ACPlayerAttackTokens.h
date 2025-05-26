// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ACPlayerAttackTokens.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SURAS_API UACPlayerAttackTokens : public UActorComponent
{
	GENERATED_BODY()
	
	int _AttackTokens = 8;
	int _MaxAttackTokens = 8;

public:
	// Sets default values for this component's properties
	UACPlayerAttackTokens();
	
	bool ReserveAttackToken(int ReserveAmount);
	void ReturnAttackToken(int ReserveAmount);
};
