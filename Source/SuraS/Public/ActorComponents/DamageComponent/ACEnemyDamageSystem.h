// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ACDamageSystem.h"
#include "NiagaraSystem.h"
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

	UPROPERTY(EditAnywhere)
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

	TArray<float> ImpulsePower;
	float CalculateImpulsePower(float Damage, float MaxHealth);

	void PartBroken(AActor* OwningEnemyActor, const FDamageData& DamageData, float PartMaxHealth, const FName PartsParent, TSubclassOf<AActor> SeparatedPart = nullptr);

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

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> LLeg;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> RLeg;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	UNiagaraSystem* BloodEffect;

	FORCEINLINE void SetImpulsePower(float weak, float normal, float hard);
	FORCEINLINE void SetPartsHealth(float head, float body, float rArm, float lArm, float lLeg, float rLeg);
};
