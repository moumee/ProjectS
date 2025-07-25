// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponAction.h"
#include "BufferedFireRequest.generated.h"

/**
 *
 */
USTRUCT(Atomic, BlueprintType)
struct FBufferedFireRequest
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditAnywhere)
	EWeaponAction ActionName;
	UPROPERTY(EditAnywhere)
	bool bIsLeftInput;
	UPROPERTY(EditAnywhere)
	bool bSingleProjectile;
	UPROPERTY(EditAnywhere)
	int32 NumPenetrable;


	FBufferedFireRequest(
		EWeaponAction InActionName = EWeaponAction::WeaponAction_SingleShot,
		bool InbIsLeftInput = true,
		bool InbSingleProjectile = true,
		int32 InNumPenetrable = 0
	) :
		ActionName(InActionName),
		bIsLeftInput(InbIsLeftInput),
		bSingleProjectile(InbSingleProjectile),
		NumPenetrable(InNumPenetrable)
	{
	}
};

UCLASS()
class SURAS_API ABufferedFireRequestStruct : public AActor
{
	GENERATED_BODY()
public:
	ABufferedFireRequestStruct();
};