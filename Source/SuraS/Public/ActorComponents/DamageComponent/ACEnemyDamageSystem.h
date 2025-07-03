// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ACDamageSystem.h"
#include "ACEnemyDamageSystem.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SURAS_API UACEnemyDamageSystem : public UACDamageSystem
{
	GENERATED_BODY()

	float HeadHealth = 100.f;
	float MaxHeadHealth = 100.f;
	
	float BodyHealth = 100.f;
	float MaxBodyHealth = 100.f;
	
	float RArmHealth = 100.f;
	float MaxRArmHealth = 100.f;
	
	float LArmHealth = 100.f;
	float MaxLArmHealth = 100.f;
	
	float RLegHealth = 100.f;
	float MaxRLegHealth = 100.f;
	
	float LLegHealth = 100.f;
	float MaxLLegHealth = 100.f;

	static TSet<FName> HeadBoneNames;
	static TSet<FName> LArmBoneNames;
	static TSet<FName> RArmBoneNames;
	static TSet<FName> LLegBoneNames;
	static TSet<FName> RLegBoneNames;

public:
	// Sets default values for this component's properties
	UACEnemyDamageSystem();
	
	virtual bool TakeDamage(const FDamageData& DamageData, const AActor* DamageCauser) override;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> RArm;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> LArm;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> Head;
};
