// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/Boss/Services/BTS_CheckAttackAreaTag.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"
#include "Characters/Enemies/Boss/SuraCharacterBossProto.h"

UBTS_CheckAttackAreaTag::UBTS_CheckAttackAreaTag()
{
	NodeName = "Check Attack Area Tag";

	INIT_SERVICE_NODE_NOTIFY_FLAGS();
}

void UBTS_CheckAttackAreaTag::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	if (UBlackboardData* BBAsset = GetBlackboardAsset())
	{
		AttackAreaTagKey.ResolveSelectedKey(*BBAsset);
	}
}

void UBTS_CheckAttackAreaTag::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ASuraCharacterBossProto* Boss = Cast<ASuraCharacterBossProto>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Boss) return;

	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	BlackboardComponent->SetValueAsName(AttackAreaTagKey.SelectedKeyName, Boss->GetCurrentAttackAreaTag());
}
