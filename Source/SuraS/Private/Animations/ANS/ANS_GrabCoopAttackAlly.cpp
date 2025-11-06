// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/ANS/ANS_GrabCoopAttackAlly.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemies/SuraCharacterEnemyBase.h"
#include "Characters/Enemies/AI/EnemyBaseAIController.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

#define ECC_ENEMY_OVERLAP ECC_GameTraceChannel5
#define ECC_ENEMY_PAWN ECC_GameTraceChannel6

void UANS_GrabCoopAttackAlly::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                          float TotalDuration, const FAnimNotifyEventReference& EvetnRef)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EvetnRef);

	if (ASuraCharacterEnemyBase* OwnerEnemy = Cast<ASuraCharacterEnemyBase>(MeshComp->GetOwner()))
	{
		if (ASuraCharacterEnemyBase* AllyEnemy = Cast<ASuraCharacterEnemyBase>(OwnerEnemy->GetAIController()->GetBlackboardComponent()->GetValueAsObject("CoopAlly")))
		{
			OwnerEnemy->GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_ENEMY_OVERLAP, ECollisionResponse::ECR_Ignore);
			OwnerEnemy->GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_ENEMY_PAWN, ECollisionResponse::ECR_Ignore);
			OwnerEnemy->GetMesh()->IgnoreActorWhenMoving(AllyEnemy, true);
			OwnerEnemy->GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_ENEMY_OVERLAP, ECollisionResponse::ECR_Ignore);
			OwnerEnemy->GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_ENEMY_PAWN, ECollisionResponse::ECR_Ignore);
			OwnerEnemy->GetCapsuleComponent()->IgnoreActorWhenMoving(AllyEnemy, true);
			
			FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
			AllyEnemy->AttachToComponent(OwnerEnemy->GetMesh(), AttachmentRules, FName(TEXT("RightHand")));
		}
	}
}

void UANS_GrabCoopAttackAlly::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EvetnRef)
{
	Super::NotifyEnd(MeshComp, Animation, EvetnRef);

	if (ASuraCharacterEnemyBase* OwnerEnemy = Cast<ASuraCharacterEnemyBase>(MeshComp->GetOwner()))
	{
		if (ASuraCharacterEnemyBase* AllyEnemy = Cast<ASuraCharacterEnemyBase>(OwnerEnemy->GetAIController()->GetBlackboardComponent()->GetValueAsObject("CoopAlly")))
		{
			if (ASuraPawnPlayer* Player = Cast<ASuraPawnPlayer>(OwnerEnemy->GetAIController()->GetBlackboardComponent()->GetValueAsObject("AttackTarget")))
			{
				FDetachmentTransformRules DetachmentRules(EDetachmentRule::KeepWorld, true);
				AllyEnemy->DetachFromActor(DetachmentRules);
		
				FVector AttackTargetLocation = Player->GetActorLocation();
				FVector FinalDestination = FVector(AttackTargetLocation.X, AttackTargetLocation.Y, AttackTargetLocation.Z + 250.f);
				FVector LaunchVelocity = FinalDestination - AllyEnemy->GetActorLocation();

				// CachedEnemy->GetCharacterMovement()->SafeMveUpdateComponent
				// UGameplayStatics::SuggestProjectileVelocity_CustomArc(this, LaunchVelocity, CachedEnemyAlly->GetActorLocation(), FinalDestination, 0.f, 0.5f);
		
				AllyEnemy->LaunchCharacter(LaunchVelocity * 3.f, true, true);
			}
		}
	}
}
