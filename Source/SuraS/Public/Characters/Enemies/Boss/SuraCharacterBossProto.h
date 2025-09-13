// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "Characters/Enemies/Boss/SuraCharacterBossBase.h"
#include "Components/TimelineComponent.h"
#include "SuraCharacterBossProto.generated.h"

class UBlackboardComponent;
enum class EBossState : uint8;
class ASuraBossAttackArea;
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

	void SetCurrentState(EBossState NewState);

	EBossState GetCurrentState() const { return CurrentState; }

	ASuraBossAttackArea* GetAttackAreaByTag(FName Tag);

	
protected:

	UPROPERTY(EditAnywhere)
	TObjectPtr<UPhysicsAsset> LeftArmPhysicsAsset;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UPhysicsAsset> RightArmPhysicsAsset;

	UPROPERTY(VisibleAnywhere, Category="Attack")
	TArray<AActor*> AttackAreas;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> HitMontage;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> ArmDismemberMontage;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> DeathMontage;

	void OnArmDismemberMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	
	void PlayHitMontage(FName SectionName);

	void PlayArmDismemberMontage(FName SectionName);
	
	void DismemberArm(USkeletalMeshComponent* PartMesh, USkeletalMeshComponent* LeaderMesh, FName HideBoneName);

	UFUNCTION()
	void OnBossPartDestroyed(TEnumAsByte<EPhysicalSurface> PhysicalSurface);
	
	void OnBossDeath();
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UBlackboardComponent> BlackboardComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EBossState CurrentState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMeshComponent> HeadMesh;
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
