// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_GetYeeted.generated.h"

class ASuraCharacterEnemyBase;
/**
 * 
 */
UCLASS()
class SURAS_API UBTT_GetYeeted : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	UPROPERTY()
	ASuraCharacterEnemyBase* CachedEnemy;

	bool bIsFalling = false;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:
	explicit UBTT_GetYeeted(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
