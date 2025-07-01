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
	float RecoilAmountX = 1.5f;

	UPROPERTY(EditAnywhere)
	float RecoilRangeMinX = 0.8f;

	UPROPERTY(EditAnywhere)
	float RecoilRangeMaxX = 1.2f;

	UPROPERTY(EditAnywhere)
	float RecoilAmountY = 0.8f;

	UPROPERTY(EditAnywhere)
	float RecoilRangeMinY = -1.f;

	UPROPERTY(EditAnywhere)
	float RecoilRangeMaxY = 1.f;

	UPROPERTY(EditAnywhere)
	float RecoilAmountZ = 0.8f;

	UPROPERTY(EditAnywhere)
	float RecoilRangeMinZ = -1.f;

	UPROPERTY(EditAnywhere)
	float RecoilRangeMaxZ = 1.f;

	UPROPERTY(EditAnywhere)
	float RecoilSpeed = 4.f;
	UPROPERTY(EditAnywhere)
	float RecoilRecoverSpeed = 3.5f;

	FArmRecoilStruct(
		bool bIsRecoilRecoverAffectedByInput = true,
		float InRecoilAmountX = 1.5f,
		float InRecoilRangeMinX = 0.8f,
		float InRecoilRangeMaxX = 1.2f,
		float InRecoilAmountY = 0.8f,
		float InRecoilRangeMinY = -1.f,
		float InRecoilRangeMaxY = 1.f,
		float InRecoilAmountZ = 0.8f,
		float InRecoilRangeMinZ = -1.f,
		float InRecoilRangeMaxZ = 1.f,
		float InRecoilSpeed = 4.f,
		float InRecoilRecoverSpeed = 3.5f
		):
		//bIsRecoilRecoverAffectedByPlayerInput(bIsRecoilRecoverAffectedByInput),
		RecoilAmountX(InRecoilAmountX),
		RecoilRangeMinX(InRecoilRangeMinX),
		RecoilRangeMaxX(InRecoilRangeMaxX),
		RecoilAmountY(InRecoilAmountY),
		RecoilRangeMinY(InRecoilRangeMinY),
		RecoilRangeMaxY(InRecoilRangeMaxY),
		RecoilAmountZ(InRecoilAmountZ),
		RecoilRangeMinZ(InRecoilRangeMinZ),
		RecoilRangeMaxZ(InRecoilRangeMaxZ),
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
