// Fill out your copyright notice in the Description page of Project Settings.


#include "Utilities/EnemyClimbNavLink.h"

#include "ActorComponents/DamageComponent/ACEnemyDamageSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemies/SuraCharacterEnemyBase.h"
#include "Characters/Enemies/AI/EnemyBaseAIController.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AEnemyClimbNavLink::AEnemyClimbNavLink()
{
	SetSmartLinkEnabled(true);
	bSmartLinkIsRelevant = true;
}

// Called when the game starts or when spawned
void AEnemyClimbNavLink::BeginPlay()
{
	Super::BeginPlay();

	OnSmartLinkReached.AddDynamic(this, &AEnemyClimbNavLink::OnReceiveSmartLinkReached);
}

void  AEnemyClimbNavLink::OnReceiveSmartLinkReached(AActor* Agent, const FVector& Destination)
{
	// let the enemy jump off the ledge if on the higher ground
	if (Destination.Z < Agent->GetActorLocation().Z)
		return;

	// have the enemy climb up the wall
	if (ASuraCharacterEnemyBase* const Enemy = Cast<ASuraCharacterEnemyBase>(Agent))
	{
		// Enemy->Climb(Destination);

		CachedEnemy = Enemy;
		CachedEnemy->GetDamageSystemComp()->OnDeath.AddUObject(this, &AEnemyClimbNavLink::OnCachedEnemyDeath); // if enemy dies while climbing up, re-enable the link

		SetLinkUsable(false);
		
		CachedEnemy->GetAIController()->ClearFocus(EAIFocusPriority::Gameplay);
		
		if (CachedEnemy->GetAIController()->GetCurrentState() == EEnemyStates::Pursue || CachedEnemy->GetAIController()->GetCurrentState() == EEnemyStates::Attacking)
			CachedEnemy->GetAIController()->EndPursueState();

		CachedEnemy->GetMovementComponent()->StopMovementImmediately(); // prevent unwanted climbing due to movement
		
		CachedEnemy->GetAIController()->UpdateCurrentState(EEnemyStates::Climbing);
		CachedEnemy->GetAIController()->GetBlackboardComponent()->SetValueAsVector("TargetLocation", Destination); // to straighten the climbing rotation
		
		FRotator TargetYawRotation = UKismetMathLibrary::FindLookAtRotation(GetActorTransform().TransformPosition(PointLinks[0].Left), GetActorTransform().TransformPosition(PointLinks[0].Right));
		FRotator TargetRotation = FRotator(0, TargetYawRotation.Yaw, 0);

		CachedEnemy->GetAIController()->GetBlackboardComponent()->SetValueAsRotator("TargetRotation", TargetRotation); // to straighten the climbing rotation
		
		// CachedEnemy->SetActorLocation(GetActorTransform().TransformPosition(PointLinks[0].Left));
		CachedEnemy->SetActorRotation(TargetRotation);
	}
}

void AEnemyClimbNavLink::OnCachedEnemyDeath()
{
	SetLinkUsable(true);
}

void AEnemyClimbNavLink::SetLinkUsable(bool bIsUsable)
{
	SetSmartLinkEnabled(bIsUsable);

	if (!bIsUsable)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Enemy Climb disabled"));

	if (bIsUsable)
		CachedEnemy = nullptr;
}