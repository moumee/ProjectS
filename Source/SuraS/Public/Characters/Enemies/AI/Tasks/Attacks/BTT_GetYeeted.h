// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_GetYeeted.generated.h"

class ASuraCharacterEnemyBase;
/**
 * 
 */
struct FBTTGetYeetedTaskMemory
{
	bool bIsFalling = false;
	bool bIsDoneGettingYeeted =false;
	
	TWeakObjectPtr<ASuraCharacterEnemyBase> CachedEnemy;
};

UCLASS()
class SURAS_API UBTT_GetYeeted : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	virtual uint16 GetInstanceMemorySize() const override;
	
public:
	explicit UBTT_GetYeeted(FObjectInitializer const& ObjectInitializer);
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
