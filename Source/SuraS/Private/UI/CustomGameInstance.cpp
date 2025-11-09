// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CustomGameInstance.h"

#include "Instance/SuraCheckpointSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGame/SettingSaveGame.h"
#include "SaveGame/SuraSaveGame.h"

UCustomGameInstance::UCustomGameInstance()
{
	SettingsSaveSlotName = TEXT("SettingsSaveSlot");
	MouseSensitivity = 0.7f; // default value for game instance;
}

void UCustomGameInstance::Init()
{
	Super::Init();
	LoadSettings();

	USuraCheckpointSubsystem* CheckpointSubsystem = GetSubsystem<USuraCheckpointSubsystem>();
	if (CheckpointSubsystem)
	{
		const FString SlotName = CheckpointSubsystem->GetCheckpointSlotName(); 
        
		if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
		{
			// 동기식으로 저장 파일을 불러옵니다.
			USuraSaveGame* LoadedSave = Cast<USuraSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
			if (LoadedSave)
			{
				// 서브시스템의 CurrentSave 변수에 로드한 데이터를 설정합니다.
				CheckpointSubsystem->SetCurrentSave(LoadedSave); 
				UE_LOG(LogTemp, Warning, TEXT("GameInstance::Init: 저장된 체크포인트(%s) 로드 성공."), *SlotName);
			}
		}
	}
}

void UCustomGameInstance::Shutdown()
{
	SaveSettings();
	
	USuraCheckpointSubsystem* CheckpointSubsystem = GetSubsystem<USuraCheckpointSubsystem>();
	if (CheckpointSubsystem)
	{
		CheckpointSubsystem->SaveOnQuit();
	}
	
	Super::Shutdown();
}

// --- 옵션 UI에서 호출할 공용 함수 ---

void UCustomGameInstance::ApplyAndSaveSettings()
{
	SaveSettings();
}


void UCustomGameInstance::SaveSettings()
{
	USettingSaveGame* SaveGameInstance = Cast<USettingSaveGame>(UGameplayStatics::CreateSaveGameObject(USettingSaveGame::StaticClass()));
	if (SaveGameInstance)
	{
		// 현재 gameinstance에 저장된 값을 savegame 객체에 복사
		SaveGameInstance->SavedMouseSensitivity = this->MouseSensitivity;

		UGameplayStatics::SaveGameToSlot(SaveGameInstance, SettingsSaveSlotName, 0);
	}
}

void UCustomGameInstance::LoadSettings()
{
	USettingSaveGame* LoadGameInstance = nullptr;

	if (UGameplayStatics::DoesSaveGameExist(SettingsSaveSlotName, 0))
	{
		LoadGameInstance = Cast<USettingSaveGame>(UGameplayStatics::LoadGameFromSlot(SettingsSaveSlotName, 0));
	}

	if (LoadGameInstance)
	{
		MouseSensitivity = LoadGameInstance->SavedMouseSensitivity;
	}
	else
	{
		SaveSettings();
	}
}
