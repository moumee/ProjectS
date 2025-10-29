// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PlayerSound_DataAsset.generated.h"

/**
 * 
 */
UCLASS()
class SURAS_API UPlayerSound_DataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category="Hit|Boss")
	TObjectPtr<USoundBase> BossNormalHitSound;

	UPROPERTY(EditAnywhere, Category="Hit|Boss")
	TObjectPtr<USoundBase> BossDownedHitSound;

	UPROPERTY(EditAnywhere, Category="Hit|Melee")
	TObjectPtr<USoundBase> MeleeEnemyHitSound;

	UPROPERTY(EditAnywhere, Category="Hit|Rifle")
	TObjectPtr<USoundBase> RifleEnemyHitSound;

	UPROPERTY(EditAnywhere, Category="Hit|Charger")
	TObjectPtr<USoundBase> ChargerEnemyHitSound;

	UPROPERTY(EditAnywhere, Category="Hit|Turret")
	TObjectPtr<USoundBase> TurretEnemyHitSound;

	UPROPERTY(EditAnywhere, Category="Movement")
	TObjectPtr<USoundBase> PrimaryJumpSound;
	UPROPERTY(EditAnywhere, Category="Movement")
	TObjectPtr<USoundBase> DoubleJumpSound;
	UPROPERTY(EditAnywhere, Category="Movement")
	TObjectPtr<USoundBase> WallRunSound;
	UPROPERTY(EditAnywhere, Category="Movement")
	TObjectPtr<USoundBase> SlideSound;
	UPROPERTY(EditAnywhere, Category="Movement")
	TObjectPtr<USoundBase> LandSound;

	
};
