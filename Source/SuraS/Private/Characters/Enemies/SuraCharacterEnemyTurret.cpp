// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/SuraCharacterEnemyTurret.h"

#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
ASuraCharacterEnemyTurret::ASuraCharacterEnemyTurret()
{
	EnemyType = "Turret";
}

// Called when the game starts or when spawned
void ASuraCharacterEnemyTurret::BeginPlay()
{
	Super::BeginPlay();
	
	GetCharacterMovement()->SetMovementMode(MOVE_Flying);
}