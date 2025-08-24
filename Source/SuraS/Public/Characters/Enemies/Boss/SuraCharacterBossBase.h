// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/SuraCharacterBase.h"
#include "Interfaces/Damageable.h"
#include "SuraCharacterBossBase.generated.h"

#define SURFACE_HEAD SurfaceType6
#define SURFACE_BODY SurfaceType7
#define SURFACE_LEFT_ARM SurfaceType8
#define SURFACE_RIGHT_ARM SurfaceType9

class UACBossDamageSystem;
/**
 * 
 */
UCLASS()
class SURAS_API ASuraCharacterBossBase : public ASuraCharacterBase, public IDamageable
{
	GENERATED_BODY()

public:
	
	ASuraCharacterBossBase();
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE UACBossDamageSystem* GetDamageSystemComp() { return DamageSystemComponent; }

	virtual bool TakeDamage(const FDamageData& DamageData, AActor* DamageCauser) override;

	virtual void Destroyed() override;
protected:

	UPROPERTY()
	TObjectPtr<UACBossDamageSystem> DamageSystemComponent;

	

	
	
};
