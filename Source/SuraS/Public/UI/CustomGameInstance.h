// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ActorComponents/WeaponSystem/WeaponName.h"
#include "CustomGameInstance.generated.h"

class USettingSaveGame;

/**
 * 
 */
UCLASS()
class SURAS_API UCustomGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UCustomGameInstance();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings")
	float MouseSensitivity = 0.f;

	// 무기 소유 상태를 저장하는 TMap
	UPROPERTY()
	TMap<EWeaponName, bool> OwnedWeapons;

protected:
	

	// --- 무기 저장 관련 ---
	FString WeaponSaveSlotName;
	void SaveWeaponData();
	void LoadWeaponData();

	// --- 설정 저장 관련 ---
	FString SettingsSaveSlotName;
	void SaveSettings();
	void LoadSettings();

public:
	virtual void Init() override; // 게임 시작 시 자동 호출 됨.
	virtual void Shutdown() override; // 게임 종료시 호출.

	/** 옵션 UI에서 이 함수를 호출하여 설정을 즉시 저장 */
	UFUNCTION(BlueprintCallable, Category = "Settings")
	void ApplyAndSaveSettings();
	
	
};
