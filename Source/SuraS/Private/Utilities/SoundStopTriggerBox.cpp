// Fill out your copyright notice in the Description page of Project Settings.


#include "Utilities/SoundStopTriggerBox.h"

#include "Interfaces/PlayerInterface.h"
#include "Utilities/SoundStartTriggerBox.h"


// Sets default values
ASoundStopTriggerBox::ASoundStopTriggerBox()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ASoundStopTriggerBox::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASoundStopTriggerBox::OnTriggerBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (!Cast<IPlayerInterface>(OtherActor)) return;

	if (ASoundStartTriggerBox* Trigger = SoundTriggerToStop.Get())
	{
		
	}
}


