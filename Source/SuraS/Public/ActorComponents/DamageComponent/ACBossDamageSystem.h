// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponents/DamageComponent/ACDamageSystem.h"
#include "ACBossDamageSystem.generated.h"

/**
 * 
 */
UCLASS()
class SURAS_API UACBossDamageSystem : public UACDamageSystem
{
	GENERATED_BODY()

	

public:
	
	virtual bool TakeDamage(const FDamageData& DamageData, AActor* DamageCauser) override;
};
