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

		// [추가] 무기 소지 현황 저장
		ASuraPawnPlayer* PlayerPawn = Cast<ASuraPawnPlayer>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
		if (PlayerPawn)
		{
			UWeaponSystemComponent* WeaponSystem = PlayerPawn->GetWeaponSystemComponent();
			if (WeaponSystem)
			{
				// WeaponSystemComponent의 실제 무기 인벤토리(AWeapon* 배열)를 순회
				for (AWeapon* Weapon : WeaponSystem->GetWeaponInventory())
				{
					if (Weapon)
					{
						// TMap<EWeaponName, bool>에 저장
						SaveGameInstance->OwnedWeapons.Add(Weapon->GetWeaponName(), true);
					}
				}
			}
		}

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



