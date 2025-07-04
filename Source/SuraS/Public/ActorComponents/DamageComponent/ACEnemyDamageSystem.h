// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ACDamageSystem.h"
#include "ACEnemyDamageSystem.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SURAS_API UACEnemyDamageSystem : public UACDamageSystem
{
	GENERATED_BODY()

	float HeadHealth = 300.f;
	float MaxHeadHealth = 300.f;
	
	float BodyHealth = 100.f;
	float MaxBodyHealth = 100.f;
	
	float RArmHealth = 100.f;
	float MaxRArmHealth = 100.f;
	
	float LArmHealth = 100.f;
	float MaxLArmHealth = 100.f;
	
	float RLegHealth = 400.f;
	float MaxRLegHealth = 400.f;
	
	float LLegHealth = 400.f;
	float MaxLLegHealth = 400.f;

	static TSet<FName> HeadBoneNames;
	static TSet<FName> LArmBoneNames;
	static TSet<FName> RArmBoneNames;
	static TSet<FName> LLegBoneNames;
	static TSet<FName> RLegBoneNames;
	FActorSpawnParameters SpawnParams;

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
