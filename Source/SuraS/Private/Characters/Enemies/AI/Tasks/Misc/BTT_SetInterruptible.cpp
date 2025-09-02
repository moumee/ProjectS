// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/AI/Tasks/Misc/BTT_SetInterruptible.h"

#include "AIController.h"
#include "ActorComponents/DamageComponent/ACEnemyDamageSystem.h"
#include "Characters/Enemies/SuraCharacterEnemyBase.h"

UBTT_SetInterruptible::UBTT_SetInterruptible(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Set Interruptible";
}

EBTNodeResult::Type UBTT_SetInterruptible::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (ASuraCharacterEnemyBase* const Enemy = Cast<ASuraCharacterEnemyBase>(OwnerComp.GetAIOwner()->GetCharacter()))
	{
		Enemy->GetDamageSystemComp()->SetIsInterruptible(bShouldBeInterruptible);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
