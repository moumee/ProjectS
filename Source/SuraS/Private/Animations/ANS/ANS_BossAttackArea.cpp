// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/ANS/ANS_BossAttackArea.h"

#include "Characters/Enemies/Boss/SuraBossAttackArea.h"
#include "Characters/Enemies/Boss/SuraCharacterBossProto.h"

void UANS_BossAttackArea::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                      float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	BossRef = Cast<ASuraCharacterBossProto>(MeshComp->GetOwner());
	if (BossRef)
	{
		switch (DamageType)
		{
			case EDamageType::Charge:
				BossRef->SpecialAttackArea->SetAttackAreaCollision(ECollisionEnabled::Type::QueryOnly);
				break;
			default:
				BossRef->NormalAttackArea->SetAttackAreaCollision(ECollisionEnabled::Type::QueryOnly);
				break;
		}
	}
	
}

void UANS_BossAttackArea::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (BossRef)
	{
		switch (DamageType)
		{
			case EDamageType::Charge:
				BossRef->SpecialAttackArea->SetAttackAreaCollision(ECollisionEnabled::Type::NoCollision);
				break;
			default:
				BossRef->NormalAttackArea->SetAttackAreaCollision(ECollisionEnabled::Type::NoCollision);
				break;
		}
	}
}
