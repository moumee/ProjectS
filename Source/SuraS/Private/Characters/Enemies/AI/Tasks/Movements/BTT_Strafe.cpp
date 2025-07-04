// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/AI/Tasks/Movements/BTT_Strafe.h"

#include "Characters/Enemies/SuraCharacterEnemyBase.h"
#include "Characters/Enemies/AI/EnemyBaseAIController.h"

UBTT_Strafe::UBTT_Strafe(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Strafe";
	bNotifyTick = true;

	bCreateNodeInstance = true; // prevent all AI sharing the same instance with the same StrafeDirection value
}

EBTNodeResult::Type UBTT_Strafe::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (FMath::RandRange(0, 1))
		StrafeDirection = 1;
	else
		StrafeDirection = -1;

	StrafeDuration += FMath::RandRange(-StrafeDurationDeviation, StrafeDurationDeviation);

	if (StrafeDuration < 0.f)
		StrafeDuration = 0.f;
	
	FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
	return EBTNodeResult::InProgress;
}

void UBTT_Strafe::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (ASuraCharacterEnemyBase* const Enemy = Cast<ASuraCharacterEnemyBase>(OwnerComp.GetAIOwner()->GetCharacter()))
		Enemy->AddMovementInput(StrafeDirection * Enemy->GetActorRightVector(), 0.2f);

	DeltaTime += DeltaSeconds;
	
	if (DeltaTime >= StrafeDuration)
	{
		DeltaTime = 0.f;
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
