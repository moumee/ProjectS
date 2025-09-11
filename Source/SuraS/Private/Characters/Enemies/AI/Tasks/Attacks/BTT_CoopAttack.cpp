// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/AI/Tasks/Attacks/BTT_CoopAttack.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemies/SuraCharacterEnemyBase.h"
#include "Kismet/GameplayStatics.h"

UBTT_CoopAttack::UBTT_CoopAttack(FObjectInitializer const& ObjectInitializer)
{
}

EBTNodeResult::Type UBTT_CoopAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (ASuraCharacterEnemyBase* const Enemy = Cast<ASuraCharacterEnemyBase>(OwnerComp.GetAIOwner()->GetCharacter()))
	{
		OnAttackMontageEnded.BindUObject(this, &UBTT_CoopAttack::OnAttackEnded, &OwnerComp);

		UAnimInstance* const EnemyAnimInstance = Enemy->GetMesh()->GetAnimInstance();
		UAnimMontage* CoopAttackMontage = Enemy->GetCoopAttackMontage();

		EnemyAnimInstance->Montage_Play(CoopAttackMontage);
		EnemyAnimInstance->Montage_SetEndDelegate(OnAttackMontageEnded, CoopAttackMontage);
	}

	return EBTNodeResult::InProgress;
}

void UBTT_CoopAttack::OnAttackEnded(UAnimMontage* AnimMontage, bool bInterrupted, UBehaviorTreeComponent* OwnerComp)
{
	if (ASuraCharacterEnemyBase* const EnemyAlly = Cast<ASuraCharacterEnemyBase>(OwnerComp->GetBlackboardComponent()->GetValueAsObject("CoopAlly")))
	{
		if (ASuraPawnPlayer* const Player = Cast<ASuraPawnPlayer>(OwnerComp->GetBlackboardComponent()->GetValueAsObject("AttackTarget")))
		{
			FVector LaunchVelocity = FVector::ZeroVector;
			FVector AttackTargetLocation = Player->GetActorLocation();
			FVector FinalDestination = FVector(AttackTargetLocation.X, AttackTargetLocation.Y, AttackTargetLocation.Z + 250.f);
	
			UGameplayStatics::SuggestProjectileVelocity_CustomArc(this, LaunchVelocity, EnemyAlly->GetActorLocation(), FinalDestination);

			EnemyAlly->LaunchCharacter(LaunchVelocity, true, true);
		}
	}
	
	FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
}