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

	CachedEnemy = Cast<ASuraCharacterEnemyBase>(MeshComp->GetOwner());

	if (CachedEnemy)
	{
		CachedEnemyAlly = Cast<ASuraCharacterEnemyBase>(CachedEnemy->GetAIController()->GetBlackboardComponent()->GetValueAsObject("CoopAlly"));
		Player = Cast<ASuraPawnPlayer>(CachedEnemy->GetAIController()->GetBlackboardComponent()->GetValueAsObject("AttackTarget"));

		if (CachedEnemyAlly)
		{
			CachedEnemy->GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_ENEMY_OVERLAP, ECollisionResponse::ECR_Ignore);
			CachedEnemy->GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_ENEMY_PAWN, ECollisionResponse::ECR_Ignore);
			CachedEnemy->GetMesh()->IgnoreActorWhenMoving(CachedEnemyAlly, true);
			CachedEnemy->GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_ENEMY_OVERLAP, ECollisionResponse::ECR_Ignore);
			CachedEnemy->GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_ENEMY_PAWN, ECollisionResponse::ECR_Ignore);
			CachedEnemy->GetCapsuleComponent()->IgnoreActorWhenMoving(CachedEnemyAlly, true);
			
			FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
			CachedEnemyAlly->AttachToComponent(CachedEnemy->GetMesh(), AttachmentRules, FName(TEXT("RightHand")));
		}
	}
}

void UANS_GrabCoopAttackAlly::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EvetnRef)
{
	Super::NotifyEnd(MeshComp, Animation, EvetnRef);

	if (CachedEnemy && CachedEnemyAlly && Player)
	{
		FDetachmentTransformRules DetachmentRules(EDetachmentRule::KeepWorld, true);
		CachedEnemyAlly->DetachFromActor(DetachmentRules);
		
		FVector AttackTargetLocation = Player->GetActorLocation();
		FVector FinalDestination = FVector(AttackTargetLocation.X, AttackTargetLocation.Y, AttackTargetLocation.Z + 250.f);
		FVector LaunchVelocity = FinalDestination - CachedEnemyAlly->GetActorLocation();

		// CachedEnemy->GetCharacterMovement()->SafeMveUpdateComponent
		// UGameplayStatics::SuggestProjectileVelocity_CustomArc(this, LaunchVelocity, CachedEnemyAlly->GetActorLocation(), FinalDestination, 0.f, 0.5f);
		
		CachedEnemyAlly->LaunchCharacter(LaunchVelocity * 3.f, true, true);
	}
}
