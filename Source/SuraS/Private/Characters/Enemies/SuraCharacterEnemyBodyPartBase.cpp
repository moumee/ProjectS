// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/SuraCharacterEnemyBodyPartBase.h"

void ASuraCharacterEnemyBodyPartBase::BeginPlay()
{
	Super::BeginPlay();
	if (auto SkeletalMesh = GetComponentByClass<USkeletalMeshComponent>())
	{
		UE_LOG(LogTemp, Error, TEXT("f"));
		SkeletalMesh->SetCollisionProfileName(TEXT("Ragdoll"));
		SkeletalMesh->SetSimulatePhysics(true);
	}
}

// Sets default values
ASuraCharacterEnemyBodyPartBase::ASuraCharacterEnemyBodyPartBase()
{
	InitialLifeSpan = 5.0f;
	
}

