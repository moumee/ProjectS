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
	if (_EnemyAttackTokens >= ReserveAmount)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Reserved Attack Tokens: %d"), _EnemyAttackTokens));
		_EnemyAttackTokens -= ReserveAmount;
		return true;
	}
	
	return false;
}

void UACPlayerAttackTokens::ReturnAttackToken(int ReserveAmount)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Returned Attack Tokens: %d"), _EnemyAttackTokens));
	_EnemyAttackTokens += ReserveAmount;

	if (_EnemyAttackTokens > _MaxEnemyAttackTokens)
		_EnemyAttackTokens = _MaxEnemyAttackTokens;
}

/**
 * @brief Returns remaining number of Pursuit Tokens that will be used as the index of the pursuit state enemy. Returns -1 if no token left.
 */
int UACPlayerAttackTokens::ReservePursuitToken(int ReserveAmount)
{
	if (_EnemyPursuitTokens >= ReserveAmount)
	{
		_EnemyPursuitTokens -= ReserveAmount;
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Reserved Pursuit Tokens: %d"), _EnemyPursuitTokens));
		
		return _EnemyPursuitTokens;
	}
	
	return -1;
}

void UACPlayerAttackTokens::ReturnPursuitToken(int ReserveAmount)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Returned Pursuit Tokens: %d"), _EnemyPursuitTokens));
	_EnemyPursuitTokens += ReserveAmount;

	if (_EnemyPursuitTokens > _MaxEnemyPursuitTokens)
		_EnemyPursuitTokens = _MaxEnemyPursuitTokens;
}

