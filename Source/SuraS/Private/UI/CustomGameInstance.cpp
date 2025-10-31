// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CustomGameInstance.h"

#include "Kismet/GameplayStatics.h"
#include "SaveGame/SettingSaveGame.h"
#include "UI/WeaponSaveGame.h"

UCustomGameInstance::UCustomGameInstance()
{
	// c++ 생성자에서 기본값 초기화
	WeaponSaveSlotName = TEXT("WeaponSaveSlot");
	SettingsSaveSlotName = TEXT("SettingsSaveSlot");
	MouseSensitivity = 0.7f; // default value for game instance;
}

void UCustomGameInstance::Init()
{
	Super::Init();
	LoadWeaponData();
	LoadSettings();
}

void UCustomGameInstance::Shutdown()
{
	SaveWeaponData();  // 게임 종료 시 자동 저장
	SaveSettings();
	
	Super::Shutdown();
}

// --- 옵션 UI에서 호출할 공용 함수 ---

void UCustomGameInstance::ApplyAndSaveSettings()
{
	SaveSettings();
}

// --- 내부 설정 저장/로드 로직 ---

void UCustomGameInstance::SaveWeaponData()
{
	UWeaponSaveGame* SaveGameInstance = Cast<UWeaponSaveGame>(UGameplayStatics::CreateSaveGameObject(UWeaponSaveGame::StaticClass()));
	if (SaveGameInstance)
	{
		for (const auto& Elem : OwnedWeapons)
		{
			SaveGameInstance->OwnedWeapons.Add(Elem.Key, Elem.Value);
		}
		UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("WeaponSaveSlot"), 0);
	}
}

void UCustomGameInstance::LoadWeaponData()
{
	if (UGameplayStatics::DoesSaveGameExist(TEXT("WeaponSaveSlot"), 0))
	{
		UWeaponSaveGame* LoadGameInstance = Cast<UWeaponSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("WeaponSaveSlot"), 0));
		if (LoadGameInstance)
		{
			OwnedWeapons = LoadGameInstance->OwnedWeapons;
		}
	}
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
		// 로드 실패 시 생성장에서 설정한 기본값(0.7f)유지
		// 이 기본값으로 세이브 파일을 만듦
		SaveSettings();
	}
}
