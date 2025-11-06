// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/ANS/ANS_BossRangedAttack.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Characters/Enemies/Boss/SuraCharacterBossProto.h"
#include "Characters/PawnBasePlayer/SuraPawnPlayer.h"
#include "Kismet/KismetSystemLibrary.h"

static TAutoConsoleVariable<bool> CVarShowBossRangeAttack(TEXT("moumee.ShowBossRangeAttack"), false, TEXT("Shows boss ranged debug shape"));

void UANS_BossRangedAttack::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                        float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);
	
	
	ASuraCharacterBossProto* Boss = Cast<ASuraCharacterBossProto>(MeshComp->GetOwner());
	if (!IsValid(Boss)) return;
	
	Boss->GetLaserNiagaraComponent()->DeactivateImmediate();
	Boss->GetLaserNiagaraComponent()->AttachToComponent(Boss->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "Muzzle");
	checkf(Boss->GetLaserFireNiagaraSystem(), TEXT("Laser fire niagara system is not fucking assigned in boss blueprint!!!"));
		
	UNiagaraComponent* SpawnedFireEffect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(Boss, Boss->GetLaserFireNiagaraSystem(),
		Boss->LastSavedMuzzlePositionBeforeFire, FRotator::ZeroRotator, FVector(1), true, true,
		ENCPoolMethod::AutoRelease, true);
	
	if (SpawnedFireEffect)
	{
		//TODO: Enable this line when effect is done.
	
		
		//SpawnedFireEffect->SetVariableVec3("User.BeamEnd", Boss->GetLaserFireEnd());
	}
	
	
}

void UANS_BossRangedAttack::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	ASuraCharacterBossProto* Boss = Cast<ASuraCharacterBossProto>(MeshComp->GetOwner());
	if (!IsValid(Boss)) return;

	if (Boss->GetMeleeHitPlayer()) return;
	
	FVector LaserStart = Boss->LastSavedMuzzlePositionBeforeFire;
	FVector LaserEnd = Boss->GetLaserFireEnd();

	FHitResult LaserHit;
	EDrawDebugTrace::Type DebugTraceType = CVarShowBossRangeAttack.GetValueOnGameThread() ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None;
	bool bHit = UKismetSystemLibrary::SphereTraceSingleByProfile(MeshComp, LaserStart, LaserEnd, 30.f, FName("Player"),
		false, {}, DebugTraceType, LaserHit, true);

	if (bHit)
	{
		if (ASuraPawnPlayer* Player = Cast<ASuraPawnPlayer>(LaserHit.GetActor()))
		{
			Boss->SetMeleeHitPlayer(true);
			FDamageData DamageData;
			DamageData.DamageType = EDamageType::Projectile;
			DamageData.ImpactPoint = LaserHit.ImpactPoint;
			DamageData.BoneName = LaserHit.BoneName;
			DamageData.bCanForceDamage = false;
			DamageData.DamageAmount = Boss->GetRangedDamageAmount(); 
			Player->TakeDamage(DamageData, MeshComp->GetOwner());
		}
	}

	
}

void UANS_BossRangedAttack::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);
	ASuraCharacterBossProto* Boss = Cast<ASuraCharacterBossProto>(MeshComp->GetOwner());
	if (!IsValid(Boss)) return;

	Boss->SetMeleeHitPlayer(false);
}