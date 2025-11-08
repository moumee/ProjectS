// Fill out your copyright notice in the Description page of Project Settings.


#include "Utilities/SoundStartTriggerBox.h"

#include "Components/AudioComponent.h"
#include "Components/ShapeComponent.h"
#include "Interfaces/PlayerInterface.h"

#define PLAYER_TRACE_CHANNEL ECC_GameTraceChannel4

// Sets default values
ASoundStartTriggerBox::ASoundStartTriggerBox()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	GetCollisionComponent()->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
	GetCollisionComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetCollisionComponent()->SetCollisionResponseToChannel(PLAYER_TRACE_CHANNEL, ECR_Overlap);
	

	BackgroundMusicAudioComponent = CreateDefaultSubobject<UAudioComponent>("BackgroundMusicAudioComponent");
	BackgroundMusicAudioComponent->SetupAttachment(RootComponent);
	BackgroundMusicAudioComponent->bAutoActivate = false;
	BackgroundMusicAudioComponent->bAllowSpatialization = false;

	SpatialEffectAudioComponent = CreateDefaultSubobject<UAudioComponent>("SpatialEffectAudioComponent");
	SpatialEffectAudioComponent->SetupAttachment(RootComponent);
	SpatialEffectAudioComponent->bAutoActivate = false;
}

// Called when the game starts or when spawned
void ASoundStartTriggerBox::BeginPlay()
{
	Super::BeginPlay();
	
	SetActorHiddenInGame(false);
	OnActorBeginOverlap.AddDynamic(this, &ThisClass::ASoundStartTriggerBox::OnTriggerBeginOverlap);
	
}

void ASoundStartTriggerBox::OnTriggerBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (!Cast<IPlayerInterface>(OtherActor)) return;
	
	if (BackgroundMusicAudioComponent->GetSound())
	{
		BackgroundMusicAudioComponent->FadeIn(BackgroundMusicFadeInDuration, 1.f, 0.f, EAudioFaderCurve::Logarithmic);
	}

	if (SpatialEffectAudioComponent->GetSound())
	{
		SpatialEffectAudioComponent->FadeIn(SpatialEffectFadeInDuration, 1.f, 0.f, EAudioFaderCurve::Logarithmic);
	}
}

