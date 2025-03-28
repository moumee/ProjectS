// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SuraEnemyWeapon.generated.h"

UCLASS()
class SURAS_API ASuraEnemyWeapon : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASuraEnemyWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY()
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, Category = "Weapon Mesh")
	USkeletalMesh* WeaponMeshAsset;
};
