// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/Firearms/SuraFirearmBase.h"
#include "SuraFirearmMissileLauncher.generated.h"

/**
 * 
 */
UCLASS()
class SURAS_API ASuraFirearmMissileLauncher : public ASuraFirearmBase
{
	GENERATED_BODY()

public:
	virtual void Fire(AActor* FirearmOwner, const AActor* TargetActor) override;
};
