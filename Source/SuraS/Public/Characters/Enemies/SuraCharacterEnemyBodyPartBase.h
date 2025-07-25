// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SuraCharacterEnemyBodyPartBase.generated.h"

UCLASS()
class SURAS_API ASuraCharacterEnemyBodyPartBase : public AActor
{
	GENERATED_BODY()

	void BeginPlay() override;
	
public:	
	// Sets default values for this actor's properties
	ASuraCharacterEnemyBodyPartBase();
};
