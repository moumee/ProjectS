// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/AttackComponents/ACPlayerAttackTokens.h"


// Sets default values for this component's properties
UACPlayerAttackTokens::UACPlayerAttackTokens()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

bool UACPlayerAttackTokens::ReserveAttackToken(int ReserveAmount)
{
	if (_AttackTokens >= ReserveAmount)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Reserved Attack Tokens: %d"), _AttackTokens));
		_AttackTokens -= ReserveAmount;
		return true;
	}
	
	return false;
}

void UACPlayerAttackTokens::ReturnAttackToken(int ReserveAmount)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Returned Attack Tokens: %d"), _AttackTokens));
	_AttackTokens += ReserveAmount;

	if (_AttackTokens > _MaxAttackTokens)
		_AttackTokens = _MaxAttackTokens;
}

