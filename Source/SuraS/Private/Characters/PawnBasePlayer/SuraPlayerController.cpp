// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PawnBasePlayer/SuraPlayerController.h"

#include "Camera/CameraComponent.h"
#include "Characters/PawnBasePlayer/SuraPawnPlayer.h"


void ASuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	SuraPawnPlayer = Cast<ASuraPawnPlayer>(GetPawn());
	
}

void ASuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

}


