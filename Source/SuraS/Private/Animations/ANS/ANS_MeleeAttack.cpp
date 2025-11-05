// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/ANS/ANS_MeleeAttack.h"
#include "Components/CapsuleComponent.h"
#include "Structures/DamageData.h"
#include "Enumerations/EDamageType.h"
#include "Characters/Player/SuraCharacterPlayer.h"

void UANS_MeleeAttack::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EvetnRef)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EvetnRef);

	if (ASuraCharacterEnemyBase* OwnerEnemy = Cast<ASuraCharacterEnemyBase>(MeshComp->GetOwner()))
		OwnerEnemy->SetCanInflictDamage(true);
}

void UANS_MeleeAttack::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EvetnRef)
{
	Super::NotifyTick(MeshComp, Animation, TotalDuration, EvetnRef);

	ASuraCharacterEnemyBase* OwnerEnemy = Cast<ASuraCharacterEnemyBase>(MeshComp->GetOwner());

	if (OwnerEnemy && MeshComp)
	{
		TArray<FHitResult> Hits;
		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.AddIgnoredActor(OwnerEnemy);

		const FVector Start = OwnerEnemy->GetActorLocation() + OwnerEnemy->GetActorForwardVector() * OwnerEnemy->GetCapsuleComponent()->GetScaledCapsuleRadius();
		const FVector End = Start + OwnerEnemy->GetActorForwardVector() * OwnerEnemy->GetMeleeAttackRange();

		// ANS cannot access GetWorld directly! Must be accessed through MeshComp or whoever has access to GetWorld function first
		bool bHit = MeshComp->GetWorld()->SweepMultiByChannel(
			Hits,
			Start,
			End,
			FQuat::Identity,
			ECollisionChannel::ECC_Pawn,
			FCollisionShape::MakeSphere(OwnerEnemy->GetMeleeAttackSphereRadius()),
			CollisionQueryParams
		);

		if (bHit && OwnerEnemy->CanInflictDamage())
		{
			for (auto Hit : Hits)
			{
				if (ASuraPawnPlayer* const Player = Cast<ASuraPawnPlayer>(Hit.GetActor()))
				{

					FDamageData DamageData;
					DamageData.DamageAmount = OwnerEnemy->GetAttackDamageAmount() + AdditionalDamageAmount;
					DamageData.DamageType = EDamageType::Melee;

					Player->TakeDamage(DamageData, OwnerEnemy);

					OwnerEnemy->SetCanInflictDamage(false);

					break;
				}
			}
		}

		// ANS cannot access GetWorld directly
		// DrawDebugSphere(MeshComp->GetWorld(), Start + (End - Start) * 0.5f, AttackRadius, 24, Hit.bBlockingHit ? FColor::Blue : FColor::Red, false, 5.0f, 0, 10.0f);
	}
}

void UANS_MeleeAttack::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EvetnRef)
{
	Super::NotifyEnd(MeshComp, Animation, EvetnRef);

	if (ASuraCharacterEnemyBase* OwnerEnemy = Cast<ASuraCharacterEnemyBase>(MeshComp->GetOwner()))
		OwnerEnemy->SetCanInflictDamage(true);
}
