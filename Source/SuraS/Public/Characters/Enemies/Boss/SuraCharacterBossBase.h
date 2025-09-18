// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/SuraCharacterBase.h"
#include "Interfaces/Damageable.h"
#include "SuraCharacterBossBase.generated.h"



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
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UACBossDamageSystem> DamageSystemComponent;

	

	
	
};
