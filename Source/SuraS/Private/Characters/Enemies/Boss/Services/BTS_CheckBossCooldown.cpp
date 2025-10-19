// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/Boss/Services/BTS_CheckBossCooldown.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemies/Boss/SuraCharacterBossProto.h"

UBTS_CheckBossCooldown::UBTS_CheckBossCooldown()
{
	NodeName = "Check Boss Cooldown";
	
	INIT_SERVICE_NODE_NOTIFY_FLAGS();
}

void UBTS_CheckBossCooldown::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ASuraCharacterBossProto* Boss = Cast<ASuraCharacterBossProto>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Boss) return;

	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	BlackboardComponent->SetValueAsBool(MeleeCooldownKey.SelectedKeyName, Boss->GetIsMeleeAttackOnCooldown());
	BlackboardComponent->SetValueAsBool(RangedCooldownKey.SelectedKeyName, Boss->GetIsRangedAttackOnCooldown());
	 
}

void UBTS_CheckBossCooldown::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	if (UBlackboardData* BBAsset = GetBlackboardAsset())
	{
		MeleeCooldownKey.ResolveSelectedKey(*BBAsset);
		RangedCooldownKey.ResolveSelectedKey(*BBAsset);
	}
}
