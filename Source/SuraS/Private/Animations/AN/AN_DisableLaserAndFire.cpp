// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/AN/AN_DisableLaserAndFire.h"

#include "NiagaraComponent.h"
#include "Characters/Enemies/Boss/SuraCharacterBossProto.h"
#include "Characters/PawnBasePlayer/SuraPawnPlayer.h"
#include "Kismet/KismetSystemLibrary.h"

void UAN_DisableLaserAndFire::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                     const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	AActor* Actor = MeshComp->GetOwner();
	if (!IsValid(Actor)) return;

	ASuraCharacterBossProto* Boss = Cast<ASuraCharacterBossProto>(Actor);
	if (!IsValid(Boss)) return;

	check(Boss->GetLaserNiagaraComponent());
	Boss->GetLaserNiagaraComponent()->DeactivateImmediate();

	FVector LaserStart = Boss->GetMesh()->GetSocketLocation(FName("Muzzle"));
	FVector LaserEnd = Boss->GetLaserFireEnd();

	FHitResult LaserHit;
	bool bHit = UKismetSystemLibrary::SphereTraceSingleByProfile(MeshComp, LaserStart, LaserEnd, 3.f, FName("Player"),
		false, {}, EDrawDebugTrace::ForOneFrame, LaserHit, true);

	if (bHit)
	{
		if (ASuraPawnPlayer* Player = Cast<ASuraPawnPlayer>(LaserHit.GetActor()))
		{
			FDamageData DamageData;
			DamageData.DamageType = EDamageType::Projectile;
			DamageData.ImpactPoint = LaserHit.ImpactPoint;
			DamageData.BoneName = LaserHit.BoneName;
			DamageData.bCanForceDamage = false;
			DamageData.DamageAmount = 20.f; // TODO: Make this to a variable
			Player->TakeDamage(DamageData, MeshComp->GetOwner());
		}
	}
}
