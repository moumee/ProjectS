// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/SuraCharacterEnemyCharger.h"


// Sets default values
ASuraCharacterEnemyCharger::ASuraCharacterEnemyCharger()
{
	EnemyType = "Charger";
}

// Called when the game starts or when spawned
void ASuraCharacterEnemyCharger::BeginPlay()
{
	Super::BeginPlay();
}

void ASuraCharacterEnemyCharger::Attack(ASuraPawnPlayer* Player)
{
	Super::Attack(Player);
}