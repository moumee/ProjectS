// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/AN/AN_LungeToTarget.h"

#include "Characters/Enemies/SuraCharacterEnemyBase.h"

void UAN_LungeToTarget::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (ASuraCharacterEnemyBase* Owner = Cast<ASuraCharacterEnemyBase>(MeshComp->GetOwner()))
		Owner->LungeToTarget(LungeForce);
}
