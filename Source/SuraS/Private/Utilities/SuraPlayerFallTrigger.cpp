// Fill out your copyright notice in the Description page of Project Settings.


#include "Utilities/SuraPlayerFallTrigger.h"

#include "Characters/PawnBasePlayer/SuraPawnPlayer.h"
#include "Components/ShapeComponent.h"
#include "GameModes/SuraLevelGameMode.h"
#include "Kismet/GameplayStatics.h"

#define PLAYER_TRACE_CHANNEL ECC_GameTraceChannel4

// Sets default values
ASuraPlayerFallTrigger::ASuraPlayerFallTrigger()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	GetCollisionComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetCollisionComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetCollisionComponent()->SetCollisionResponseToChannel(PLAYER_TRACE_CHANNEL, ECR_Overlap);
}

// Called when the game starts or when spawned
void ASuraPlayerFallTrigger::BeginPlay()
{
	Super::BeginPlay();

	OnActorBeginOverlap.AddDynamic(this, &ThisClass::OnTriggerBeginOverlap);
}

void ASuraPlayerFallTrigger::OnTriggerBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (ASuraPawnPlayer* Player = Cast<ASuraPawnPlayer>(OtherActor))
	{
		ASuraLevelGameMode* GameMode = Cast<ASuraLevelGameMode>(UGameplayStatics::GetGameMode(this));
		check(GameMode);

		GameMode->OnPlayerFellOutOfWorld(Player);
	}
}


