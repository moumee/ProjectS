// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Characters/Enemies/Boss/SuraBoss_DataAsset.h"
#include "BTT_BossMeleeAttack.generated.h"

class ASuraCharacterBossProto;
/**
 * 
 */
UCLASS()
class SURAS_API UBTT_BossMeleeAttack : public UBTTaskNode
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector AttackAreaKey;
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector MeleeCooldownKey;

	UPROPERTY()
	TWeakObjectPtr<ASuraCharacterBossProto> BossRef;

	FOnMontageEnded OnMontageEndedDelegate;

	void OnMontageEnded(UAnimMontage* AnimMontage, bool bInterrupted, UBehaviorTreeComponent* OwnerComp);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	
	
	
};
