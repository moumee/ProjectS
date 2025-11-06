// Fill out your copyright notice in the Description page of Project Settings.


#include "Utilities/SoundStopTriggerBox.h"


// Sets default values
ASoundStopTriggerBox::ASoundStopTriggerBox()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASoundStopTriggerBox::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASoundStopTriggerBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

