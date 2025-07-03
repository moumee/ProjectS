// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArmRecoilStruct.generated.h"

/**
 * 
 */
USTRUCT(Atomic, BlueprintType)
struct FArmRecoilStruct
{
	GENERATED_USTRUCT_BODY()
public:
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//bool bIsRecoilRecoverAffectedByPlayerInput = true;
	UPROPERTY(EditAnywhere)
	FVector RecoilAmount;

	UPROPERTY(EditAnywhere)
	FVector RecoilRangeMin;

	UPROPERTY(EditAnywhere)
	FVector RecoilRangeMax;

	//UPROPERTY(EditAnywhere)
	//float RecoilAmountX = 1.5f;

	//UPROPERTY(EditAnywhere)
	//float RecoilRangeMinX = 0.8f;

	//UPROPERTY(EditAnywhere)
	//float RecoilRangeMaxX = 1.2f;

	//UPROPERTY(EditAnywhere)
	//float RecoilAmountY = 0.8f;

	//UPROPERTY(EditAnywhere)
	//float RecoilRangeMinY = -1.f;

	//UPROPERTY(EditAnywhere)
	//float RecoilRangeMaxY = 1.f;

	//UPROPERTY(EditAnywhere)
	//float RecoilAmountZ = 0.8f;

	//UPROPERTY(EditAnywhere)
	//float RecoilRangeMinZ = -1.f;

	//UPROPERTY(EditAnywhere)
	//float RecoilRangeMaxZ = 1.f;

	UPROPERTY(EditAnywhere)
	float RecoilSpeed = 4.f;
	UPROPERTY(EditAnywhere)
	float RecoilRecoverSpeed = 3.5f;

	FArmRecoilStruct(
		FVector InRecoilAmount = {1.f, 1.f, 1.f},
		FVector InRecoilRangeMin = { 1.f, 1.f, 1.f },
		FVector InRecoilRangeMax = { 1.f, 1.f, 1.f },
		float InRecoilSpeed = 4.f,
		float InRecoilRecoverSpeed = 3.5f
		):
		RecoilAmount(InRecoilAmount),
		RecoilRangeMin(InRecoilRangeMin),
		RecoilRangeMax(InRecoilRangeMax),
		RecoilSpeed(InRecoilSpeed),
		RecoilRecoverSpeed(InRecoilRecoverSpeed)
	{
	}
};


UCLASS()
class SURAS_API AArmRecoilValueStruct : public AActor
{
	GENERATED_BODY()
public:
	AArmRecoilValueStruct();
};
