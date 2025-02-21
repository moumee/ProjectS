// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CustomGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SURAS_API UCustomGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init() override; // 게임 시작 시 자동 호출 됨.
	virtual void Shutdown() override; // 게임 종료시 호출.

	// 무기 데이터 저장 및 불러오기
	void SaveWeaponData();
	void LoadWeaponData();

	// 무기 데이터 테이블 (DT_Weapon)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	UDataTable* DTWeapon;
	
	// 무기 소유 상태를 저장하는 TMap
	UPROPERTY()
	TMap<FName, bool> OwnedWeapons;

	
	
};
