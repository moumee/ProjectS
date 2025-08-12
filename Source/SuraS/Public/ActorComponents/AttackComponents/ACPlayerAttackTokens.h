// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ACPlayerAttackTokens.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SURAS_API UACPlayerAttackTokens : public UActorComponent
{
	GENERATED_BODY()
	
	int _EnemyAttackTokens = 8;
	int _MaxEnemyAttackTokens = 8;

	int _EnemyPursuitTokens = 8;
	int _MaxEnemyPursuitTokens = 8;

public:
	// Sets default values for this component's properties
	UACPlayerAttackTokens();
	
	bool ReserveAttackToken(int ReserveAmount);
	void ReturnAttackToken(int ReserveAmount);

	int ReservePursuitToken(int ReserveAmount);
	void ReturnPursuitToken(int ReserveAmount);

	FORCEINLINE int GetMaxEnemyPursuitTokens() const { return _MaxEnemyPursuitTokens; };

	void SetMaxEnemyAttackTokens(int32 MaxTokens);

	void SetMaxEnemyPursuitTokens(int32 MaxTokens);
};
