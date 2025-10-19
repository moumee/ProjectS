// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SuraBossStates.h"
#include "SuraBoss_DataAsset.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "Characters/Enemies/Boss/SuraCharacterBossBase.h"
#include "Components/TimelineComponent.h"
#include "SuraCharacterBossProto.generated.h"

class USuraBoss_DataAsset;
class UBlackboardComponent;
enum class EBossState : uint8;
class ASuraBossAttackArea;
class UTimelineComponent;
/**
 * 
 */

USTRUCT(BlueprintType)
struct FBossMeleeInfo
{
	GENERATED_BODY()

	UPROPERTY()
	TObjectPtr<UAnimMontage> AttackMontage;
	UPROPERTY()
	float Cooldown;
	
};

UCLASS()
class SURAS_API ASuraCharacterBossProto : public ASuraCharacterBossBase
{
	GENERATED_BODY()

public:
	ASuraCharacterBossProto();
	
	virtual void Tick(float DeltaTime) override;

	virtual bool TakeDamage(const FDamageData& DamageData, AActor* DamageCauser) override;

	void SetCurrentState(EBossState NewState);

	EBossState GetCurrentState() const { return CurrentState; }

	void SetCurrentArmState(EBossArmState NewState);

	EBossArmState GetCurrentArmState() const { return CurrentArmState; }

	void GetAttackAreasByTag(FName Tag, TArray<ASuraBossAttackArea*>& OutAreas);

	FBossMeleeInfo GetMeleeAttackMontageAndCooldownByTag(FName Tag);

	bool GetIsMeleeAttackOnCooldown() const { return bIsMeleeAttackOnCooldown; }
	bool GetIsRangedAttackOnCooldown() const { return bIsRangedAttackOnCooldown; }

	void StartMeleeAttackCooldown(float Duration);
	void StartRangedAttackCooldown(float Duration);

	void AddAttackAreaTag(FName InTag);

	void RemoveAttackAreaTag(FName InTag);

	FName GetCurrentAttackAreaTag() const;
	
protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USuraBoss_DataAsset> BossDataAsset;

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
	EBossState CurrentState = EBossState::Idle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMeshComponent> HeadMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMeshComponent> LeftArmMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<USkeletalMeshComponent> RightArmMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EBossArmState CurrentArmState = EBossArmState::HasBothArms;
	
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

	UPROPERTY(VisibleAnywhere)
	TArray<FBossMeleeAttack> MeleeAttacks;

	UPROPERTY(VisibleAnywhere, Category="Cooldown")
	float MeleeAttackCooldown = 0.f;
	UPROPERTY(VisibleAnywhere, Category="Cooldown")
	bool bIsMeleeAttackOnCooldown = false;
	UPROPERTY(VisibleAnywhere, Category="Cooldown")
	float RangedAttackCooldown = 0.f;
	UPROPERTY(VisibleAnywhere, Category="Cooldown")
	bool bIsRangedAttackOnCooldown = false;

	TSet<FName> AttackAreaTags;
};
