// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Delegates/Delegate.h"
#include "Components/ActorComponent.h"
#include "ACDamageSystem.generated.h"

struct FDamageData;

// Event dispatchers
DECLARE_MULTICAST_DELEGATE(FOnDamaged);
DECLARE_MULTICAST_DELEGATE(FOnDeath);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SURAS_API UACDamageSystem : public UActorComponent
{
	GENERATED_BODY()

protected:
	float Health = 100.f;
	float MaxHealth = 100.f;
	bool bIsInterruptible = true;
	bool bIsInvincible = false;
	bool bIsDead = false;

public:	
	// Sets default values for this component's properties
	UACDamageSystem();

	// Setters
	void SetHealth(float Health);
	void SetMaxHealth(float MaxHealth);
	void SetIsInterruptible(bool bIsInterruptible);
	void SetIsInvincible(bool bIsInvincible);
	void SetIsDead(bool bIsDead);

	// Getters
	float GetHealth() const { return Health; }
	float GetMaxHealth() const { return MaxHealth; }
	bool GetIsInterruptible() const { return bIsInterruptible; }

	FOnDamaged OnDamaged;
	FOnDeath OnDeath;

	UFUNCTION(BlueprintCallable)
	virtual bool TakeDamage(const FDamageData& DamageData, AActor* DamageCauser);
};
