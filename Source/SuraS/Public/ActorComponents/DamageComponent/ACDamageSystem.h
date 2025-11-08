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
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChangedSignature, float, CurrentHealth, float, OldHealth, float, MaxHealth, AActor*, DamageCauser);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SURAS_API UACDamageSystem : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere)
	float Health = 100.f;
	UPROPERTY(VisibleAnywhere)
	float MaxHealth = 100.f;
	bool bIsInterruptible = true;
	bool bIsInvincible = false;
	UPROPERTY(VisibleAnywhere)
	bool bIsDead = false;

	// --- 체력 재생 로직 ---
	UPROPERTY(EditAnywhere, Category = "Health|Regeneration")
	float TimeUntilRegenStart;
	UPROPERTY(EditAnywhere, Category = "Health|Regeneration")
	float RegenHealAmount;

	FTimerHandle RegenTimerHandle;

	void StartRegeneration();
	void StartHealing();

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
	bool GetIsDead() const { return bIsDead; }
	
	UFUNCTION(BlueprintCallable, Category = "Health")
	void Heal(float HealAmount);

	UFUNCTION(BlueprintPure, Category = "Health")
	float GetHealthRatio() const;
    
	UFUNCTION(BlueprintPure, Category = "Health")
	bool IsDead() const;

	FOnDamaged OnDamaged;
	FOnDeath OnDeath;

	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnHealthChangedSignature OnHealthChanged;

	UFUNCTION(BlueprintCallable)
	virtual bool TakeDamage(const FDamageData& DamageData, AActor* DamageCauser);
};
