// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CustomGameInstance.h"

#include "ActorComponents/WeaponSystem/WeaponData.h"
#include "Kismet/GameplayStatics.h"
#include "UI/WeaponSaveGame.h"

void UCustomGameInstance::Init()
{
	Super::Init();
	LoadWeaponData();
}

void UCustomGameInstance::Shutdown()
{
	SaveWeaponData();  // 게임 종료 시 자동 저장
	Super::Shutdown();
}

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
