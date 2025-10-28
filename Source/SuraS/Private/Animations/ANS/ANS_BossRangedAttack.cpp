// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/ANS/ANS_BossRangedAttack.h"

#include "NiagaraComponent.h"
#include "Characters/Enemies/Boss/SuraCharacterBossProto.h"
#include "Characters/PawnBasePlayer/SuraPawnPlayer.h"
#include "Kismet/KismetSystemLibrary.h"

void UANS_BossRangedAttack::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                        float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	AActor* Actor = MeshComp->GetOwner();
	if (!IsValid(Actor)) return;

	WeakBoss = Cast<ASuraCharacterBossProto>(Actor);
	if (!WeakBoss.IsValid()) return;

	ASuraCharacterBossProto* Boss = WeakBoss.Get();
	Boss->GetLaserNiagaraComponent()->DeactivateImmediate();

	DamageAmount = Boss->GetRangedDamageAmount();
	
}

void UANS_BossRangedAttack::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (bHasHit) return;
	if (!WeakBoss.IsValid()) return;

	ASuraCharacterBossProto* Boss = WeakBoss.Get();
	FVector LaserStart = Boss->GetMesh()->GetSocketLocation(FName("Muzzle"));
	FVector LaserEnd = Boss->GetLaserFireEnd();

	FHitResult LaserHit;
	bool bHit = UKismetSystemLibrary::SphereTraceSingleByProfile(MeshComp, LaserStart, LaserEnd, 3.f, FName("Player"),
		false, {}, EDrawDebugTrace::None, LaserHit, true);

	if (bHit)
	{
		if (ASuraPawnPlayer* Player = Cast<ASuraPawnPlayer>(LaserHit.GetActor()))
		{
			bHasHit = true;
			FDamageData DamageData;
			DamageData.DamageType = EDamageType::Projectile;
			DamageData.ImpactPoint = LaserHit.ImpactPoint;
			DamageData.BoneName = LaserHit.BoneName;
			DamageData.bCanForceDamage = false;
			DamageData.DamageAmount = DamageAmount; 
			Player->TakeDamage(DamageData, MeshComp->GetOwner());
		}
	}
}

void UANS_BossRangedAttack::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	bHasHit = false;
}

