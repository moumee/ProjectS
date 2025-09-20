// Fill out your copyright notice in the Description page of Project Settings.


#include "Utilities/SuraCheckpoint.h"

#include "Characters/PawnBasePlayer/SuraPawnPlayer.h"
#include "Components/CapsuleComponent.h"
#include "Instance/SuraCheckpointSubsystem.h"
#include "Interfaces/PlayerInterface.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGame/SuraSaveGame.h"

#define PLAYER_TRACE_CHANNEL ECC_GameTraceChannel4


// Sets default values
ASuraCheckpoint::ASuraCheckpoint()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	GetCollisionComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	GetCollisionComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetCollisionComponent()->SetCollisionResponseToChannel(PLAYER_TRACE_CHANNEL, ECR_Overlap);

	SpawnCapsule = CreateDefaultSubobject<UCapsuleComponent>("Spawn Capsule");
	SpawnCapsule->SetupAttachment(GetRootComponent());
	SpawnCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SpawnCapsule->SetCollisionResponseToAllChannels(ECR_Ignore);
	SpawnCapsule->InitCapsuleSize(40.f, 90.f);

	OnActorBeginOverlap.AddDynamic(this, &ThisClass::OnTriggerBeginOverlap);
}

// Called when the game starts or when spawned
void ASuraCheckpoint::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASuraCheckpoint::OnTriggerBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (ASuraPawnPlayer* Player = Cast<ASuraPawnPlayer>(OtherActor))
	{
		USuraCheckpointSubsystem* Subsystem = GetGameInstance()->GetSubsystem<USuraCheckpointSubsystem>();
		check(Subsystem);
		USuraSaveGame* CurrentSaveData = Subsystem->GetCurrentSave();
		check(CurrentSaveData);

		FName CurrentMapName = FName(*UGameplayStatics::GetCurrentLevelName(this, true));
		FName SavedMapName = CurrentSaveData->MapName;
		int32 SavedOrderIndex = CurrentSaveData->CheckpointOrderIndex;
		if (CurrentMapName == SavedMapName)
		{
			if (CheckpointOrderIndex > SavedOrderIndex)
			{
				Subsystem->SaveCheckpoint(CurrentMapName, SpawnCapsule->GetComponentTransform(), CheckpointOrderIndex);
			}
		}
		else
		{
			Subsystem->SaveCheckpoint(CurrentMapName, SpawnCapsule->GetComponentTransform(), CheckpointOrderIndex);
		}

		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("Saved Checkpoint"));
	}
}


