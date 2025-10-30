// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PlayerSound_DataAsset.generated.h"

USTRUCT(BlueprintType)
struct FPlayerSoundData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> Sound;
	
	UPROPERTY(EditAnywhere)
	bool bDebug = false;
	UPROPERTY(EditAnywhere, meta=(EditCondition="bDebug"))
	float DebugDisplayDuration = 3.f;

	UPROPERTY(EditAnywhere)
	bool bMapVolume = true;
	UPROPERTY(EditAnywhere, meta=(EditCondition="bMapVolume"))
	FFloatInterval VolumeRange = FFloatInterval(0.f, 1.f);
	UPROPERTY(EditAnywhere, meta=(EditCondition="bMapVolume"))
	FFloatInterval VolumeSpeedRange = FFloatInterval(0.f, 1000.f);
	UPROPERTY(EditAnywhere)
	bool bMapPitch = true;
	UPROPERTY(EditAnywhere, meta=(EditCondition="bMapPitch"))
	FFloatInterval PitchRange = FFloatInterval(0.f, 1.f);
	UPROPERTY(EditAnywhere, meta=(EditCondition="bMapPitch"))
	FFloatInterval PitchSpeedRange = FFloatInterval(0.f, 1000.f);
};

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

	UPROPERTY(EditAnywhere, Category="Movement|Jump")
	FPlayerSoundData PrimaryJumpSound;
	UPROPERTY(EditAnywhere, Category="Movement|Jump")
	FPlayerSoundData DoubleJumpSound;
	UPROPERTY(EditAnywhere, Category="Movement|WallRun")
	FPlayerSoundData WallRunSound;
	UPROPERTY(EditAnywhere, Category="Movement|Slide")
	FPlayerSoundData SlideSound;
	UPROPERTY(EditAnywhere, Category="Movement|Land")
	FPlayerSoundData LandSound;

	
};
