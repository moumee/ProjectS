// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/AI/Tasks/Attacks/BTT_CoopAttack.h"

#include "AIController.h"
#include "MotionWarpingComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemies/SuraCharacterEnemyBase.h"
#include "Characters/Enemies/AI/EnemyBaseAIController.h"
#include "Kismet/GameplayStatics.h"

UBTT_CoopAttack::UBTT_CoopAttack()
{
	NodeName = "Coop Attack";
	bCreateNodeInstance = true;
	INIT_TASK_NODE_NOTIFY_FLAGS();
}

EBTNodeResult::Type UBTT_CoopAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	/*FBTTCoopAttackTaskMemory* Mem = CastInstanceNodeMemory<FBTTCoopAttackTaskMemory>(NodeMemory);
	check(Mem);*/
	
	if (ASuraCharacterEnemyBase* const Enemy = Cast<ASuraCharacterEnemyBase>(OwnerComp.GetAIOwner()->GetCharacter()))
	{
		// Mem->CachedEnemy = Enemy;
		CachedEnemy = Enemy;

		// Mem->CachedEnemyAlly = Cast<ASuraCharacterEnemyBase>(Enemy->GetAIController()->GetBlackboardComponent()->GetValueAsObject("CoopAlly"));
		CachedEnemyAlly = Cast<ASuraCharacterEnemyBase>(Enemy->GetAIController()->GetBlackboardComponent()->GetValueAsObject("CoopAlly"));

		/*if (Mem->CachedEnemyAlly.IsValid() && Mem->CachedEnemyAlly.Get()->GetAIController()->GetCurrentState() != EEnemyStates::CoopAttacking)
		{
			Mem->CachedEnemyAlly.Get()->GetAIController()->SetStateToCoopAttack(Enemy, false);
			Mem->CachedEnemyAlly.Get()->GetAIController()->GetBlackboardComponent()->SetValueAsRotator(
			"TargetRotation",
			FRotator(0, Mem->CachedEnemyAlly.Get()->GetActorRotation().Yaw, 0)
			);
		}*/

		if (ASuraCharacterEnemyBase* EnemyAlly = CachedEnemyAlly.Get())
		{
			if (EnemyAlly->GetAIController()->GetCurrentState() != EEnemyStates::CoopAttacking)
			{
				EnemyAlly->GetAIController()->SetStateToCoopAttack(Enemy, false);
				EnemyAlly->GetAIController()->GetBlackboardComponent()->SetValueAsRotator(
				"TargetRotation",
				FRotator(0, EnemyAlly->GetActorRotation().Yaw, 0)
				);
			}
		}
		
		OnAttackMontageEnded.BindUObject(this, &UBTT_CoopAttack::OnAttackEnded, &OwnerComp);

		UAnimInstance* const EnemyAnimInstance = Enemy->GetMesh()->GetAnimInstance();
		UAnimMontage* CoopAttackMontage = Enemy->GetCoopAttackMontage();

		EnemyAnimInstance->Montage_Play(CoopAttackMontage);
		EnemyAnimInstance->Montage_SetEndDelegate(OnAttackMontageEnded, CoopAttackMontage);
	}

	if (ASuraPawnPlayer* const Player = Cast<ASuraPawnPlayer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("AttackTarget")))
	{
		// Mem->CachedPlayer = Player;
		CachedPlayer = Player;
	}

	return EBTNodeResult::InProgress;
}

void UBTT_CoopAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	/*FBTTCoopAttackTaskMemory* Mem = CastInstanceNodeMemory<FBTTCoopAttackTaskMemory>(NodeMemory);

	if (Mem->CachedEnemy.IsValid() && Mem->CachedEnemyAlly.IsValid() && Mem->CachedPlayer.IsValid())
	{
		FRotator FaceEnemyAllyRotation = (Mem->CachedEnemyAlly.Get()->GetActorLocation() - Mem->CachedEnemy.Get()->GetActorLocation()).GetSafeNormal2D().Rotation();
		Mem->CachedEnemy.Get()->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocationAndRotation("EnemyAllyToGrab", Mem->CachedEnemy.Get()->GetActorLocation(), FaceEnemyAllyRotation);
		
		FRotator FacePlayerRotation = (Mem->CachedPlayer.Get()->GetActorLocation() - Mem->CachedEnemy.Get()->GetActorLocation()).GetSafeNormal2D().Rotation() + FRotator(0.0f, 110.0f, 0.0f);
		Mem->CachedEnemy.Get()->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocationAndRotation("AttackTargetToFace", Mem->CachedEnemy.Get()->GetActorLocation(), FacePlayerRotation);
		
		// CachedEnemy->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromComponent()
	}*/

	if (ASuraCharacterEnemyBase* Enemy = CachedEnemy.Get())
	{
		if (ASuraCharacterEnemyBase* EnemyAlly = CachedEnemyAlly.Get())
		{
			if (ASuraPawnPlayer* Player = CachedPlayer.Get())
			{
				FRotator FaceEnemyAllyRotation = (EnemyAlly->GetActorLocation() - Enemy->GetActorLocation()).GetSafeNormal2D().Rotation();
				Enemy->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocationAndRotation("EnemyAllyToGrab", Enemy->GetActorLocation(), FaceEnemyAllyRotation);
		
				FRotator FacePlayerRotation = (Player->GetActorLocation() - Enemy->GetActorLocation()).GetSafeNormal2D().Rotation() + FRotator(0.0f, 110.0f, 0.0f);
				Enemy->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromLocationAndRotation("AttackTargetToFace", Enemy->GetActorLocation(), FacePlayerRotation);
			}
		}
		
		// CachedEnemy->GetMotionWarpingComponent()->AddOrUpdateWarpTargetFromComponent()
	}
}

void UBTT_CoopAttack::OnAttackEnded(UAnimMontage* AnimMontage, bool bInterrupted, UBehaviorTreeComponent* OwnerComp)
{
	/*FBTTCoopAttackTaskMemory* Mem = CastInstanceNodeMemory<FBTTCoopAttackTaskMemory>(OwnerComp->GetNodeMemory(this, OwnerComp->FindInstanceContainingNode(this)));

	if (CachedEnemy.IsValid())
	{
		CachedEnemy.Get()->GetAIController()->GetBlackboardComponent()->SetValueAsBool("IsCoopThrower", false);
	}*/

	if (ASuraCharacterEnemyBase* Enemy = CachedEnemy.Get())
	{
		Enemy->GetAIController()->GetBlackboardComponent()->SetValueAsObject("CoopAlly", nullptr);
		Enemy->GetAIController()->GetBlackboardComponent()->SetValueAsBool("IsCoopThrower", false);
	}
	
	FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
}

/*uint16 UBTT_CoopAttack::GetInstanceMemorySize() const
{
	return sizeof(FBTTCoopAttackTaskMemory);
}*/
