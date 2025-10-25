// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/ANS/ANS_RotateTowardsAttackArea.h"
#include "Characters/Enemies/Boss/SuraBossAttackArea.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemies/Boss/SuraCharacterBossProto.h"
#include "Kismet/KismetMathLibrary.h"

void UANS_RotateTowardsAttackArea::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                               float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (ASuraCharacterBossProto* Boss = Cast<ASuraCharacterBossProto>(MeshComp->GetOwner()))
	{
		if (AAIController* AIController = Boss->GetController<AAIController>())
		{
			FName Tag = AIController->GetBlackboardComponent()->GetValueAsName("AttackAreaTag");
			TArray<ASuraBossAttackArea*> Areas;
			Boss->GetAttackAreasByTag(Tag, Areas);
			if (!Areas.IsEmpty())
			{
				FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(Boss->GetActorLocation(),
					Areas[0]->GetActorLocation());
				TargetRotation = FRotator(0.f, LookAtRotation.Yaw, 0.f);
				
			}
		}
	}
}

void UANS_RotateTowardsAttackArea::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                              float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (AActor* Actor = MeshComp->GetOwner())
	{
		FRotator InterpolatedRotation = FMath::RInterpTo(Actor->GetActorRotation(), TargetRotation, FrameDeltaTime, RotationInterpSpeed);
		Actor->SetActorRotation(InterpolatedRotation);
	}
	
}
