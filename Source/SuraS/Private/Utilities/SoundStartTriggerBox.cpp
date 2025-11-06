// Fill out your copyright notice in the Description page of Project Settings.


#include "Utilities/SoundStartTriggerBox.h"

#include "Components/AudioComponent.h"
#include "Interfaces/PlayerInterface.h"


// Sets default values
ASoundStartTriggerBox::ASoundStartTriggerBox()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BackgroundMusicAudioComponent = CreateDefaultSubobject<UAudioComponent>("BackgroundMusicAudioComponent");
	BackgroundMusicAudioComponent->SetupAttachment(RootComponent);
	BackgroundMusicAudioComponent->bAutoActivate = false;

	SpatialEffectAudioComponent = CreateDefaultSubobject<UAudioComponent>("SpatialEffectAudioComponent");
	SpatialEffectAudioComponent->SetupAttachment(RootComponent);
	SpatialEffectAudioComponent->bAutoActivate = false;
}

// Called when the game starts or when spawned
void ASoundStartTriggerBox::BeginPlay()
{
	Super::BeginPlay();

	BackgroundMusicAudioComponent->SetSound(BackgroundMusic);
	SpatialEffectAudioComponent->SetSound(SpatialEffectAudio);

	OnActorBeginOverlap.AddDynamic(this, &ThisClass::ASoundStartTriggerBox::OnTriggerBeginOverlap);
	
}

void ASoundStartTriggerBox::OnTriggerBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (!Cast<IPlayerInterface>(OtherActor)) return;
	
	if (BackgroundMusicAudioComponent->GetSound())
	{
		BackgroundMusicAudioComponent->Play();
	}

	if (SpatialEffectAudioComponent->GetSound())
	{
		SpatialEffectAudioComponent->Play();
	}
}

