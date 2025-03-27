// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/SuraPlayerBaseState.h"

#include "Characters/Player/SuraCharacterPlayer.h"


EPlayerState USuraPlayerBaseState::GetStateType() const
{
	return StateType;
}

void USuraPlayerBaseState::EnterState(ASuraCharacterPlayer* Player)
{
	
}

void USuraPlayerBaseState::UpdateState(ASuraCharacterPlayer* Player, float DeltaTime)
{
}

void USuraPlayerBaseState::ExitState(ASuraCharacterPlayer* Player)
{
	Player->ResetTriggeredBooleans();
}

void USuraPlayerBaseState::Move(ASuraCharacterPlayer* Player, const FVector2D& InputVector)
{
}

void USuraPlayerBaseState::StopMoving(ASuraCharacterPlayer* Player)
{
}

void USuraPlayerBaseState::Look(ASuraCharacterPlayer* Player, const FVector2D& InputVector)
{
}


void USuraPlayerBaseState::Landed(ASuraCharacterPlayer* Player, const FHitResult& HitResult)
{
}
