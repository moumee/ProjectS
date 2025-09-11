// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/ANS/ANS_BossAttackArea.h"

#include "ActorComponents/DamageComponent/ACDamageSystem.h"
#include "Characters/Enemies/Boss/SuraBossAttackArea.h"
#include "Characters/Enemies/Boss/SuraCharacterBossProto.h"
#include "Characters/PawnBasePlayer/SuraPawnPlayer.h"
#include "Components/BoxComponent.h"
#include "Engine/OverlapResult.h"
#include "Slate/SGameLayerManager.h"

#define PLAYER_TRACE_CHANNEL ECollisionChannel::ECC_GameTraceChannel4

void UANS_BossAttackArea::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                      float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	bHasHit = false;
	BossRef = Cast<ASuraCharacterBossProto>(MeshComp->GetOwner());
	if (!BossRef) return;
 	BossRef->AttackArea->SetAttackBoxCollision(ECollisionEnabled::Type::QueryOnly);
}

void UANS_BossAttackArea::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (!BossRef) return;
	if (!BossRef->AttackArea) return;
	if (bHasHit) return;
	
	TArray<AActor*> OverlappingActors;
	BossRef->AttackArea->GetAttackBox()->GetOverlappingActors(OverlappingActors, ASuraPawnPlayer::StaticClass());

	for (const auto& OverlappingActor : OverlappingActors)
	{
		if (IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(OverlappingActor))
		{
			bHasHit = true;
			GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("Hit"));
			FDamageData DamageData;
			DamageData.DamageType = DamageType;
			DamageData.DamageAmount = 10;

			if (DamageType == EDamageType::Charge)
			{
				GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, TEXT("Charge Hit"));
				DamageData.ImpulseMagnitude = 1000.f;
				DamageData.ImpulseDirection = (OverlappingActor->GetActorLocation() - MeshComp->GetOwner()->GetActorLocation()).GetSafeNormal2D();
			}

			if (IDamageable* Damageable = Cast<IDamageable>(OverlappingActor))
			{
				Damageable->TakeDamage(DamageData, MeshComp->GetOwner());
			}
			
		}
	}

	
}

void UANS_BossAttackArea::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (!BossRef || !BossRef->AttackArea) return;
	
	BossRef->AttackArea->SetAttackBoxCollision(ECollisionEnabled::Type::NoCollision);
}




