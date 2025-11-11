// Fill out your copyright notice in the Description page of Project Settings.


#include "Instance/SuraCheckpointSubsystem.h"

#include "ActorComponents/WeaponSystem/ACWeapon.h"
#include "ActorComponents/WeaponSystem/WeaponSystemComponent.h"
#include "Characters/PawnBasePlayer/SuraPawnPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGame/SuraSaveGame.h"


void USuraCheckpointSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	if (UGameplayStatics::DoesSaveGameExist(CheckpointSlotName, 0))
	{
		CurrentSave = Cast<USuraSaveGame>(UGameplayStatics::LoadGameFromSlot(CheckpointSlotName, 0));
	}
	else
	{
		CurrentSave = nullptr;
	}
}

void USuraCheckpointSubsystem::SetCurrentSave(USuraSaveGame* SaveGame)
{
	CurrentSave = SaveGame;
}

void USuraCheckpointSubsystem::LoadCheckpoint()
{
	CurrentSave = Cast<USuraSaveGame>(UGameplayStatics::LoadGameFromSlot(CheckpointSlotName, 0));
}

void USuraCheckpointSubsystem::SaveCheckpoint(FName MapName, const FTransform& SpawnTransform, int32 OrderIndex)
{
	CurrentSave->MapName = MapName;
	CurrentSave->SpawnTransform = SpawnTransform;
	CurrentSave->CheckpointOrderIndex = OrderIndex;
	
	UGameplayStatics::SaveGameToSlot(CurrentSave, CheckpointSlotName, 0);
	
}

bool USuraCheckpointSubsystem::HasSavedCheckpoint() const
{
	return UGameplayStatics::DoesSaveGameExist(CheckpointSlotName, 0) && CurrentSave;
}

void USuraCheckpointSubsystem::ClearSavedCheckpoint()
{
	if (UGameplayStatics::DoesSaveGameExist(CheckpointSlotName, 0))
	{
		UGameplayStatics::DeleteGameInSlot(CheckpointSlotName, 0);
	}
	
	CurrentSave = nullptr;
}


void USuraCheckpointSubsystem::SavePlayedVideo(ESuraVideo InPlayedVideo)
{
	CurrentSave->PlayedVideo = InPlayedVideo;

	UGameplayStatics::SaveGameToSlot(CurrentSave, CheckpointSlotName, 0);
}

bool USuraCheckpointSubsystem::ShouldPlayVideo(ESuraVideo VideoToCheck)
{
	if (!CurrentSave) return true;
	
	if (static_cast<uint8>(VideoToCheck) > static_cast<uint8>(CurrentSave->PlayedVideo))
	{
		return true;
	}
	
	return false;
}








