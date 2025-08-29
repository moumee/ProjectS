// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/AI/Tasks/Attacks/BTT_ChargeAttack.h"

#include "AIController.h"
#include "Characters/Enemies/SuraCharacterEnemyCharger.h"

UBTT_ChargeAttack::UBTT_ChargeAttack(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Charge Attack";
	bNotifyTick = true;

	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTT_ChargeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (ASuraCharacterEnemyCharger* const Charger = Cast<ASuraCharacterEnemyCharger>(OwnerComp.GetAIOwner()->GetCharacter()))
	{
		CachedCharger = Charger;
		
		OnAttackReadyMontageEnded.BindUObject(this, &UBTT_ChargeAttack::OnAttackReadyEnded);

		UAnimInstance* const EnemyAnimInstance = CachedCharger->GetMesh()->GetAnimInstance();
		UAnimMontage* AttackReadyAnimation = CachedCharger->GetChargeReadyAnimation();

		EnemyAnimInstance->Montage_Play(AttackReadyAnimation);

		bIsAttacking = true;

		// EnemyAnimInstance->Montage_SetBlendingOutDelegate(OnAttackReadyMontageEnded); // montage interrupted
		EnemyAnimInstance->Montage_SetEndDelegate(OnAttackReadyMontageEnded);

		return EBTNodeResult::InProgress;
	}

	return EBTNodeResult::Failed;
}

void UBTT_ChargeAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}

void UBTT_ChargeAttack::OnAttackReadyEnded(UAnimMontage* AnimMontage, bool bInterrupted)
{
	// bIsAttacking = false;
	UE_LOG(LogTemp, Warning, TEXT("Ready To Attack"));

	UAnimInstance* const EnemyAnimInstance = CachedCharger->GetMesh()->GetAnimInstance();
	UAnimMontage* AttackAnimation = CachedCharger->ChooseRandomAttackMontage();

	EnemyAnimInstance->Montage_Play(AttackAnimation, 1.f);
}