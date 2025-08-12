// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/AI/Tasks/Misc/BTT_CheckPlayerHealthCondition.h"

#include "ActorComponents/DamageComponent/ACDamageSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemies/SuraCharacterEnemyBase.h"
#include "Characters/Enemies/AI/EnemyBaseAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Structures/Enemies/EnemyAttributesData.h"

UBTT_CheckPlayerHealthCondition::UBTT_CheckPlayerHealthCondition(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Check Player Health Condition";

	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTT_CheckPlayerHealthCondition::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (ASuraPawnPlayer* const Player = Cast<ASuraPawnPlayer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("AttackTarget")))
	{
		float PlayerHealthPercentage = Player->GetDamageSystemComponent()->GetHealth() / Player->GetDamageSystemComponent()->GetMaxHealth();

		if (PlayerHealthPercentage < 0.5f)
		{
			if (AEnemyBaseAIController* const EnemyController = Cast<AEnemyBaseAIController>(OwnerComp.GetAIOwner()))
			{
				if (ASuraCharacterEnemyBase* const Enemy = Cast<ASuraCharacterEnemyBase>(OwnerComp.GetAIOwner()->GetCharacter()))
				{
					float OriginalAttackRate = EnemyController->GetBlackboardComponent()->GetValueAsFloat("AttackRate");
					float NewAttackRate = FMath::RandRange(OriginalAttackRate + Enemy->GetMinAttackRateVariation(), OriginalAttackRate + Enemy->GetMaxAttackRateVariation());

					EnemyController->GetBlackboardComponent()->SetValueAsFloat("AttackRate", NewAttackRate);

					float OriginalWalkSpeed = Enemy->GetCharacterMovement()->MaxWalkSpeed;
					Enemy->GetCharacterMovement()->MaxWalkSpeed = FMath::RandRange(OriginalWalkSpeed - Enemy->GetMaxWalkSpeedVariation(), OriginalWalkSpeed - Enemy->GetMinWalkSpeedVariation());
				}
			}
		}
		else
		{
			if (AEnemyBaseAIController* const EnemyController = Cast<AEnemyBaseAIController>(OwnerComp.GetAIOwner()))
			{
				if (ASuraCharacterEnemyBase* const Enemy = Cast<ASuraCharacterEnemyBase>(OwnerComp.GetAIOwner()->GetCharacter()))
				{
					if (const auto EnemyAttributesData = Enemy->EnemyAttributesDT.DataTable->FindRow<FEnemyAttributesData>(Enemy->GetEnemyType(), ""))
						EnemyController->GetBlackboardComponent()->SetValueAsFloat("AttackRate", EnemyAttributesData->AttackRate);

					Enemy->SetMovementSpeed(EEnemySpeed::Sprint);
				}
			}
		}
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
