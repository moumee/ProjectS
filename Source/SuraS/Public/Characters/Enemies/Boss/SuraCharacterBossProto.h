// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/Enemies/Boss/SuraCharacterBossBase.h"
#include "Components/TimelineComponent.h"
#include "SuraCharacterBossProto.generated.h"

class UTimelineComponent;
/**
 * 
 */

UCLASS()
class SURAS_API ASuraCharacterBossProto : public ASuraCharacterBossBase
{
	GENERATED_BODY()

public:
	ASuraCharacterBossProto();
	virtual void BeginPlay() override;

	virtual bool TakeDamage(const FDamageData& DamageData, AActor* DamageCauser) override;
	
protected:
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMeshComponent> HeadMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMeshComponent> BodyMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMeshComponent> LeftArmMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMeshComponent> RightArmMesh;

	
	UPROPERTY()
	TObjectPtr<UTimelineComponent> HeadHitColorTimeline;
	FOnTimelineFloat OnHeadHitColorTimelineFloat;
	UFUNCTION()
	void UpdateHeadHitColor(float Alpha);
	
	UPROPERTY()
	TObjectPtr<UTimelineComponent> BodyHitColorTimeline;
	FOnTimelineFloat OnBodyHitColorTimelineFloat;
	UFUNCTION()
	void UpdateBodyHitColor(float Alpha);
	
	UPROPERTY()
	TObjectPtr<UTimelineComponent> LeftArmHitColorTimeline;
	FOnTimelineFloat OnLeftArmHitColorTimelineFloat;
	UFUNCTION()
	void UpdateLeftArmHitColor(float Alpha);
	
	UPROPERTY()
	TObjectPtr<UTimelineComponent> RightArmHitColorTimeline;
	FOnTimelineFloat OnRightArmHitColorTimelineFloat;
	UFUNCTION()
	void UpdateRightArmHitColor(float Alpha);

	void InitializeHitColorTimelines();

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCurveFloat> HitColorCurve;
	
};
