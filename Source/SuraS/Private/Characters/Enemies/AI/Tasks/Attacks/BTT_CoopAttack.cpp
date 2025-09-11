// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/AI/Tasks/Attacks/BTT_CoopAttack.h"

#include "AIController.h"
#include "MotionWarpingComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemies/SuraCharacterEnemyBase.h"
#include "Characters/Enemies/AI/EnemyBaseAIController.h"
#include "Kismet/GameplayStatics.h"

UBTT_CoopAttack::UBTT_CoopAttack(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Coop Attack";
	bNotifyTick = true;
	
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTT_CoopAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (ASuraCharacterEnemyBase* const Enemy = Cast<ASuraCharacterEnemyBase>(OwnerComp.GetAIOwner()->GetCharacter()))
	{
		CachedEnemy = Enemy;

		CachedEnemyAlly = Cast<ASuraCharacterEnemyBase>(CachedEnemy->GetAIController()->GetBlackboardComponent()->GetValueAsObject("CoopAlly"));
		
		OnAttackMontageEnded.BindUObject(this, &UBTT_CoopAttack::OnAttackEnded, &OwnerComp);

		UAnimInstance* const EnemyAnimInstance = Enemy->GetMesh()->GetAnimInstance();
		UAnimMontage* CoopAttackMontage = Enemy->GetCoopAttackMontage();

		EnemyAnimInstance->Montage_Play(CoopAttackMontage);
		EnemyAnimInstance->Montage_SetEndDelegate(OnAttackMontageEnded, CoopAttackMontage);
	}

	if (ASuraPawnPlayer* const Player = Cast<ASuraPawnPlayer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("AttackTarget")))
	{
		CachedPlayer = Player;
	}

	return EBTNodeResult::InProgress;
}

void UBTT_CoopAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (CachedEnemy && CachedEnemyAlly && CachedPlayer)
	{
		FRotator FaceEnemyAllyRotation = (CachedEnemyAlly->GetActorLocation() - CachedEnemy->GetActorLocation()).GetSafeNormal2D().Rotation();
		CachedEnemy->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocationAndRotation("EnemyAllyToGrab", CachedEnemy->GetActorLocation(), FaceEnemyAllyRotation);
		
		FRotator FacePlayerRotation = (CachedPlayer->GetActorLocation() - CachedEnemy->GetActorLocation()).GetSafeNormal2D().Rotation() + FRotator(0.0f, 110.0f, 0.0f);
		CachedEnemy->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocationAndRotation("AttackTargetToFace", CachedEnemy->GetActorLocation(), FacePlayerRotation);
		
		// CachedEnemy->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromComponent()
	}
}

void UBTT_CoopAttack::OnAttackEnded(UAnimMontage* AnimMontage, bool bInterrupted, UBehaviorTreeComponent* OwnerComp)
{
	FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
}
