// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/SuraEnemyWeapon.h"


// Sets default values
ASuraEnemyWeapon::ASuraEnemyWeapon()
{
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ASuraEnemyWeapon::BeginPlay()
{
	Super::BeginPlay();

	if (WeaponMeshAsset)
		WeaponMesh->SetSkeletalMesh(WeaponMeshAsset);
}

