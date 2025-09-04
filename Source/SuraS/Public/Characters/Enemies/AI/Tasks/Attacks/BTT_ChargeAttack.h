// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_ChargeAttack.generated.h"

class ASuraCharacterEnemyCharger;
/**
 * 
 */
UCLASS()
class SURAS_API UBTT_ChargeAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	bool bIsAttacking = false;
	bool bWasAttackSuccessful = false;

	float OriginalMaxWalkSpeed;

	float ElapsedChargeTime = 0.f;

	UPROPERTY()
	TObjectPtr<ASuraCharacterEnemyCharger> CachedCharger;

	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> CachedOwnerComp;

	void OnAttackReadyEnded();
	void OnRoarEnded() const;
	
	void EndTask();

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	UPROPERTY(EditAnywhere)
	float ChargeDuration = 3.f;

	UPROPERTY(EditAnywhere)
	float ChargeMaxWalkSpeed = 1000.f;
	
	explicit UBTT_ChargeAttack(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
