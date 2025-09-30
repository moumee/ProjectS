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
	AttackArea = BossRef->GetAttackAreaByTag(AttackAreaTag);
	if (!AttackArea) return;
}

void UANS_BossAttackArea::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (!BossRef) return;
	if (!AttackArea) return;
	if (bHasHit) return;
	
	UBoxComponent* Box = AttackArea->GetAttackBox();
	if (!Box) return;

	FCollisionObjectQueryParams ObjParams;
	ObjParams.AddObjectTypesToQuery(PLAYER_TRACE_CHANNEL);
	const FVector BoxLocation = Box->GetComponentLocation();
	const FQuat Rot = Box->GetComponentQuat();
	const FCollisionShape Shape = FCollisionShape::MakeBox(Box->GetScaledBoxExtent());

	TArray<FOverlapResult> Hits;
	bool bHit = MeshComp->GetWorld()->OverlapMultiByObjectType(Hits, BoxLocation, Rot, ObjParams, Shape);
	if (bHit)
	{
		for (const FOverlapResult& Result : Hits)
		{
			AActor* HitActor = Result.GetActor();
			if (!HitActor) continue;
			if (IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(HitActor))
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
					DamageData.ImpulseDirection = (HitActor->GetActorLocation() - MeshComp->GetOwner()->GetActorLocation()).GetSafeNormal2D();
				}

				if (IDamageable* Damageable = Cast<IDamageable>(HitActor))
				{
					Damageable->TakeDamage(DamageData, MeshComp->GetOwner());
				}
			}
		}
	}

	
}

void UANS_BossAttackArea::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
}




