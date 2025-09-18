// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponents/DamageComponent/ACDamageSystem.h"
#include "ACBossDamageSystem.generated.h"

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

	float MaxHeadHealth;
	UPROPERTY(EditAnywhere)
	float HeadHealth;
	float MaxBodyHealth;
	UPROPERTY(EditAnywhere)
	float BodyHealth;
	float MaxLeftArmHealth;
	UPROPERTY(EditAnywhere)
	float LeftArmHealth;
	float MaxRightArmHealth;
	UPROPERTY(EditAnywhere)
	float RightArmHealth;

	UPROPERTY(VisibleAnywhere)
	bool bLeftArmDead = false;
	UPROPERTY(VisibleAnywhere)
	bool bRightArmDead = false;

	
public:

	FOnBodyPartDestroyed OnBodyPartDestroyed;
	virtual bool TakeDamage(const FDamageData& DamageData, AActor* DamageCauser) override;

	float GetLeftArmHealth() { return LeftArmHealth; }
	float GetRightArmHealth() { return RightArmHealth; }
};
