// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/SuraCharacterEnemyBodyPartBase.h"

// Sets default values
ASuraCharacterEnemyBodyPartBase::ASuraCharacterEnemyBodyPartBase()
{
	InitialLifeSpan = 5.0f;
	if (auto SkeletalMesh = GetComponentByClass<USkeletalMeshComponent>())
	{
		SkeletalMesh->SetCollisionProfileName(TEXT("Ragdoll"));
		SkeletalMesh->SetSimulatePhysics(true);
	}
}

