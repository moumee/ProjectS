// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_CoopAttack.generated.h"

class ASuraCharacterEnemyBase;
class ASuraPawnPlayer;
/**
 * 
 */
/*struct FBTTCoopAttackTaskMemory
{
	TWeakObjectPtr<ASuraCharacterEnemyBase> CachedEnemy;
	TWeakObjectPtr<ASuraCharacterEnemyBase> CachedEnemyAlly;
	TWeakObjectPtr<ASuraPawnPlayer> CachedPlayer;
};*/

UCLASS()
class SURAS_API UBTT_CoopAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	TWeakObjectPtr<ASuraCharacterEnemyBase> CachedEnemy;
	
	TWeakObjectPtr<ASuraCharacterEnemyBase> CachedEnemyAlly;
	
	TWeakObjectPtr<ASuraPawnPlayer> CachedPlayer;

	FOnMontageEnded OnAttackMontageEnded;

	void OnAttackEnded(UAnimMontage* AnimMontage, bool bInterrupted, UBehaviorTreeComponent* OwnerComp);

protected:
	// virtual uint16 GetInstanceMemorySize() const override;
	
public:
	explicit UBTT_CoopAttack();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
