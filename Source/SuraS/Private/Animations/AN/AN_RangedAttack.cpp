// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/AN/AN_RangedAttack.h"

#include "Characters/Enemies/SuraCharacterEnemyBase.h"
#include "Characters/Enemies/AI/EnemyBaseAIController.h"

void UAN_RangedAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (ASuraCharacterEnemyBase* Owner = Cast<ASuraCharacterEnemyBase>(MeshComp->GetOwner()))
		Owner->Attack(Owner->GetAIController()->GetAttackTarget());
}
