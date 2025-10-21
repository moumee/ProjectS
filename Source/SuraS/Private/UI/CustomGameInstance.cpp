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
	//// <Old Version>
	//UWeaponSaveGame* SaveGameInstance = Cast<UWeaponSaveGame>(UGameplayStatics::CreateSaveGameObject(UWeaponSaveGame::StaticClass()));

	//if (SaveGameInstance)
	//{
	//	for (const auto& Row : DTWeapon->GetRowMap())
	//	{
	//		FWeaponData* WeaponData  = (FWeaponData*)Row.Value;
	//		if (WeaponData)
	//		{
	//			SaveGameInstance->OwnedWeapons.Add(Row.Key, WeaponData->bIsWeaponOwned);
	//		}
	//	}

	//	UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("WeaponSaveSlot"), 0);
	//}

	//----------------------------------------------
	//<New Version>
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
	//// <Old Version>
	//if (UGameplayStatics::DoesSaveGameExist(TEXT("WeaponSaveSlot"), 0))
	//{
	//	UWeaponSaveGame* LoadGameInstance = Cast<UWeaponSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("WeaponSaveSlot"), 0));
	//	if (LoadGameInstance)
	//	{
	//		for (const auto& Row : DTWeapon->GetRowMap())
	//		{
	//			FWeaponData* WeaponData = (FWeaponData*)Row.Value;
	//			if (WeaponData)
	//			{
	//				if (LoadGameInstance->OwnedWeapons.Contains(Row.Key))
	//				{
	//					WeaponData->bIsWeaponOwned = LoadGameInstance->OwnedWeapons[Row.Key];
	//				}
	//			}
	//		}
	//	}
	//}

	//---------------------------------------------
	// <New Version>
	if (UGameplayStatics::DoesSaveGameExist(TEXT("WeaponSaveSlot"), 0))
	{
		UWeaponSaveGame* LoadGameInstance = Cast<UWeaponSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("WeaponSaveSlot"), 0));
		if (LoadGameInstance)
		{
			OwnedWeapons = LoadGameInstance->OwnedWeapons;
		}
	}
}
