// Fill out your copyright notice in the Description page of Project Settings.


#include "Instance/SuraCheckpointSubsystem.h"

#include "Kismet/GameplayStatics.h"
#include "SaveGame/SuraSaveGame.h"


void USuraCheckpointSubsystem::OnLoadCompleted(const FString& SlotName, const int32 UserIndex,
	USaveGame* LoadedGameData)
{
	if (USuraSaveGame* SaveData = Cast<USuraSaveGame>(LoadedGameData))
	{
		CurrentSave = SaveData;
		OnCheckpointLoadedDelegate.Broadcast();
	}
}

void USuraCheckpointSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	if (UGameplayStatics::DoesSaveGameExist(CheckpointSlotName, 0))
	{
		CurrentSave = Cast<USuraSaveGame>(UGameplayStatics::LoadGameFromSlot(CheckpointSlotName, 0));
	}
	else
	{
		CurrentSave = Cast<USuraSaveGame>(UGameplayStatics::CreateSaveGameObject(USuraSaveGame::StaticClass()));
	}
}

void USuraCheckpointSubsystem::LoadCheckpoint()
{
	FAsyncLoadGameFromSlotDelegate LoadedDelegate;
	LoadedDelegate.BindUObject(this, &ThisClass::OnLoadCompleted);
	
	if (UGameplayStatics::DoesSaveGameExist(CheckpointSlotName, 0))
	{
		UGameplayStatics::AsyncLoadGameFromSlot(CheckpointSlotName, 0, LoadedDelegate);
	}
	else
	{
		CurrentSave = Cast<USuraSaveGame>(UGameplayStatics::CreateSaveGameObject(USuraSaveGame::StaticClass()));
	}

	
}

void USuraCheckpointSubsystem::SaveCheckpoint(FName MapName, const FTransform& SpawnTransform, int32 OrderIndex)
{
	if (USuraSaveGame* SaveGameInstance = Cast<USuraSaveGame>(
		UGameplayStatics::CreateSaveGameObject(USuraSaveGame::StaticClass())))
	{
		SaveGameInstance->MapName = MapName;
		SaveGameInstance->SpawnTransform = SpawnTransform;
		SaveGameInstance->CheckpointOrderIndex = OrderIndex;

		UGameplayStatics::AsyncSaveGameToSlot(SaveGameInstance, CheckpointSlotName, 0);

		CurrentSave = SaveGameInstance;

		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("Subsystem Save!"));
	}
}

bool USuraCheckpointSubsystem::HasSavedCheckpoint() const
{
	return UGameplayStatics::DoesSaveGameExist(CheckpointSlotName, 0);
}

void USuraCheckpointSubsystem::ClearSavedCheckpoint()
{
	if (UGameplayStatics::DoesSaveGameExist(CheckpointSlotName, 0))
	{
		UGameplayStatics::DeleteGameInSlot(CheckpointSlotName, 0);
		
		CurrentSave = Cast<USuraSaveGame>(
			UGameplayStatics::CreateSaveGameObject(USuraSaveGame::StaticClass()));
	}
}



