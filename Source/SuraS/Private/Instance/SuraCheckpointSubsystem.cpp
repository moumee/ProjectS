// Fill out your copyright notice in the Description page of Project Settings.


#include "Instance/SuraCheckpointSubsystem.h"

#include "ActorComponents/WeaponSystem/ACWeapon.h"
#include "ActorComponents/WeaponSystem/WeaponSystemComponent.h"
#include "Characters/PawnBasePlayer/SuraPawnPlayer.h"
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
	
	// if (UGameplayStatics::DoesSaveGameExist(CheckpointSlotName, 0))
	// {
	// 	CurrentSave = Cast<USuraSaveGame>(UGameplayStatics::LoadGameFromSlot(CheckpointSlotName, 0));
	// }
	// else
	// {
	// 	CurrentSave = Cast<USuraSaveGame>(UGameplayStatics::CreateSaveGameObject(USuraSaveGame::StaticClass()));
	// }
}

void USuraCheckpointSubsystem::SetCurrentSave(USuraSaveGame* SaveGame)
{
	CurrentSave = SaveGame;
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
		CurrentSave = Cast<USuraSaveGame>(UGameplayStatics::CreateSaveGameObject(USuraSaveGame::StaticClass()));
		if (!CurrentSave) return;
	}
	
	CurrentSave->MapName = MapName;
	CurrentSave->SpawnTransform = SpawnTransform;
	CurrentSave->CheckpointOrderIndex = OrderIndex;
	
	ASuraPawnPlayer* PlayerPawn = Cast<ASuraPawnPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (PlayerPawn)
	{
		UWeaponSystemComponent* WeaponSystem = PlayerPawn->GetWeaponSystemComponent();
		if (WeaponSystem)
		{
			CurrentSave->OwnedWeapons = WeaponSystem->GetOwnerShipMap();
			//UE_LOG(LogTemp, Log, TEXT("[SuraCheckpointSubsystem] SaveCheckpoint: 무기 소유권 맵 복사 완료. (총 %d개 항목)"), CurrentSave->OwnedWeapons.Num());
		}
	}
	
	UGameplayStatics::AsyncSaveGameToSlot(CurrentSave, CheckpointSlotName, 0);

	//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("Subsystem Save!"));
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

void USuraCheckpointSubsystem::SaveOnQuit()
{
	if (CurrentSave)
	{
		if (CachedPlayerPawn.IsValid() && CachedWorld.IsValid())
		{
			ASuraPawnPlayer* PlayerPawn = CachedPlayerPawn.Get();
			UWorld* World = CachedWorld.Get();
			
			CurrentSave->SpawnTransform = PlayerPawn->GetActorTransform();
			CurrentSave->MapName = FName(*World->GetName());
			
			CurrentSave->OwnedWeapons.Empty(); 
			UWeaponSystemComponent* WeaponSystem = PlayerPawn->GetWeaponSystemComponent();
			if (WeaponSystem)
			{
				for (AWeapon* Weapon : WeaponSystem->GetWeaponInventory())
				{
					if (Weapon)
					{
						CurrentSave->OwnedWeapons.Add(Weapon->GetWeaponName(), true);
					}
				}
			}
		}
		
		UGameplayStatics::SaveGameToSlot(CurrentSave, CheckpointSlotName, 0);
	}
}

void USuraCheckpointSubsystem::RegisterPlayerAndWorld(ASuraPawnPlayer* PlayerPawn, UWorld* World)
{
	CachedPlayerPawn = PlayerPawn;
	CachedWorld = World;
}




