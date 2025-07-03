// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/AI/Tasks/Movements/BTT_Strafe.h"

#include "Characters/Enemies/SuraCharacterEnemyBase.h"
#include "Characters/Enemies/AI/EnemyBaseAIController.h"

UBTT_Strafe::UBTT_Strafe(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Strafe";
	bNotifyTick = true;
}

EBTNodeResult::Type UBTT_Strafe::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
	return EBTNodeResult::InProgress;
}

void UBTT_Strafe::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (ASuraCharacterEnemyBase* const Enemy = Cast<ASuraCharacterEnemyBase>(OwnerComp.GetAIOwner()->GetCharacter()))
		Enemy->AddMovementInput(Enemy->GetActorRightVector(), 0.2f);

	DeltaTime += DeltaSeconds;
	
	if (DeltaTime >= StrafeDuration)
	{
		DeltaTime = 0.f;
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
