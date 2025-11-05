// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponents/DamageComponent/ACDamageSystem.h"
#include "ACBossDamageSystem.generated.h"

struct FBossHealth;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBodyPartDestroyed, TEnumAsByte<EPhysicalSurface>, BodyPart);

/**
 * 
 */
UCLASS()
class SURAS_API UACBossDamageSystem : public UACDamageSystem
{
	GENERATED_BODY()

private:

	UACBossDamageSystem();

	float MaxHeadHealth = 0.f;
	UPROPERTY(EditAnywhere)
	float HeadHealth = 0.f;
	float MaxBodyHealth = 0.f;
	UPROPERTY(EditAnywhere)
	float BodyHealth = 0.f;
	float MaxLeftArmHealth = 0.f;
	UPROPERTY(EditAnywhere)
	float LeftArmHealth = 0.f;
	float MaxRightArmHealth = 0.f;
	UPROPERTY(EditAnywhere)
	float RightArmHealth = 0.f;

	UPROPERTY(VisibleAnywhere)
	bool bLeftArmDead = false;
	UPROPERTY(VisibleAnywhere)
	bool bRightArmDead = false;

	UPROPERTY(VisibleAnywhere)
	float HeadDamageMultiplier = 1.5f;;
	
public:

	void InitializeHealth(const FBossHealth& BossHealth);

	FOnBodyPartDestroyed OnBodyPartDestroyed;
	virtual bool TakeDamage(const FDamageData& DamageData, AActor* DamageCauser) override;

	void SetHeadDamageMultiplier(float Value) { HeadDamageMultiplier = Value; };

	float GetLeftArmHealth() const { return LeftArmHealth; }
	float GetRightArmHealth() const { return RightArmHealth; }
};
