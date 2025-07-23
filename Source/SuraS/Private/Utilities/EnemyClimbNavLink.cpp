// Fill out your copyright notice in the Description page of Project Settings.


#include "Utilities/EnemyClimbNavLink.h"

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
		
		Enemy->GetAIController()->ClearFocus(EAIFocusPriority::Gameplay);
		
		if (Enemy->GetAIController()->GetCurrentState() == EEnemyStates::Pursue || Enemy->GetAIController()->GetCurrentState() == EEnemyStates::Attacking)
			Enemy->GetAIController()->EndPursueState();

		Enemy->GetMovementComponent()->StopMovementImmediately(); // prevent unwanted climbing due to movement
		
		Enemy->GetAIController()->UpdateCurrentState(EEnemyStates::Climbing);
		Enemy->GetAIController()->GetBlackboardComponent()->SetValueAsVector("TargetLocation", Destination); // to straighten the climbing rotation
		
		FRotator TargetYawRotation = UKismetMathLibrary::FindLookAtRotation(GetActorTransform().TransformPosition(PointLinks[0].Left), GetActorTransform().TransformPosition(PointLinks[0].Right));
		FRotator TargetRotation = FRotator(Enemy->GetActorRotation().Pitch, TargetYawRotation.Yaw, Enemy->GetActorRotation().Roll);

		Enemy->GetAIController()->GetBlackboardComponent()->SetValueAsRotator("TargetRotation", TargetRotation); // to straighten the climbing rotation
		
		Enemy->SetActorLocation(GetActorTransform().TransformPosition(PointLinks[0].Left));
		Enemy->SetActorRotation(TargetRotation);
	}
}