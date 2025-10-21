// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/AI/Tasks/Movements/BTT_Strafe.h"

#include "Characters/Enemies/SuraCharacterEnemyBase.h"
#include "Characters/Enemies/AI/EnemyBaseAIController.h"

uint16 UBTT_Strafe::GetInstanceMemorySize() const
{
	return sizeof(FBTTStrafeTaskMemory);
}

UBTT_Strafe::UBTT_Strafe(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Strafe";
	INIT_TASK_NODE_NOTIFY_FLAGS();
}

EBTNodeResult::Type UBTT_Strafe::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FBTTStrafeTaskMemory* Mem = CastInstanceNodeMemory<FBTTStrafeTaskMemory>(NodeMemory);
	check(Mem);
	
	if (FMath::RandRange(0, 1))
		Mem->StrafeDirection = 1;
	else
		Mem->StrafeDirection = -1;

	StrafeDuration += FMath::RandRange(-StrafeDurationDeviation, StrafeDurationDeviation);

	if (StrafeDuration < 0.f)
		StrafeDuration = 0.f;
	
	FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
	return EBTNodeResult::InProgress;
}

void UBTT_Strafe::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	FBTTStrafeTaskMemory* Mem = CastInstanceNodeMemory<FBTTStrafeTaskMemory>(NodeMemory);

	if (ASuraCharacterEnemyBase* const Enemy = Cast<ASuraCharacterEnemyBase>(OwnerComp.GetAIOwner()->GetCharacter()))
		Enemy->AddMovementInput(Mem->StrafeDirection * Enemy->GetActorRightVector(), 0.2f);

	Mem->DeltaTime += DeltaSeconds;
	
	if (Mem->DeltaTime >= StrafeDuration)
	{
		Mem->DeltaTime = 0.f;
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
