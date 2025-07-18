// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/AI/Tasks/Movements/BTT_Climb.h"

#include "Characters/Enemies/SuraCharacterEnemyBase.h"
#include "Characters/Enemies/AI/EnemyBaseAIController.h"

UBTT_Climb::UBTT_Climb(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Climb";
	bNotifyTick = true;
}

EBTNodeResult::Type UBTT_Climb::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (ASuraCharacterEnemyBase* const Enemy = Cast<ASuraCharacterEnemyBase>(OwnerComp.GetAIOwner()->GetCharacter()))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
		return EBTNodeResult::InProgress;
	}
	
	return EBTNodeResult::Failed;
}

void UBTT_Climb::Trace()
{
	
}

void UBTT_Climb::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AActor* OwningEnemy = OwnerComp.GetAIOwner()->GetCharacter();

	// Trace to the ground in the front
	FHitResult HitRightAhead;

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(OwningEnemy);

	FVector Start = OwningEnemy->GetActorLocation();
	FVector MovementDirection = OwningEnemy->GetActorForwardVector().GetSafeNormal();
	FVector End = Start + MovementDirection * 1000.f + OwningEnemy->GetActorUpVector() * -500.f;

	bool bIsGroundRightAhead = GetWorld()->LineTraceSingleByChannel(
		HitRightAhead,
		Start,
		End,
		ECollisionChannel::ECC_Visibility,
		CollisionQueryParams
	);

	if (bIsGroundRightAhead)
	{
		DrawDebugLine(GetWorld(), Start, HitRightAhead.Location, FColor::Red);
		
		FHitResult HitFurtherAhead;
		
		End = Start + MovementDirection * 1100.f + OwningEnemy->GetActorUpVector() * -500.f;
		
		bool bIsGroundFurtherAhead = GetWorld()->LineTraceSingleByChannel(
			HitFurtherAhead,
			Start,
			End,
			ECollisionChannel::ECC_Visibility,
			CollisionQueryParams
		);

		if (bIsGroundFurtherAhead)
		{
			DrawDebugLine(GetWorld(), Start, HitFurtherAhead.Location, FColor::Red);
			
			FHitResult HitRightAheadSide;

			End = Start + MovementDirection * 1000.f + OwningEnemy->GetActorUpVector() * -500.f + MovementDirection.RotateAngleAxis(90.f, OwningEnemy->GetActorUpVector()) * 100.f;

			bool bIsGroundRightAheadSide = GetWorld()->LineTraceSingleByChannel(
				HitRightAheadSide,
				Start,
				End,
				ECollisionChannel::ECC_Visibility,
				CollisionQueryParams
			);

			if (bIsGroundRightAheadSide)
			{
				DrawDebugLine(GetWorld(), Start, HitRightAheadSide.Location, FColor::Red);
			}
		}
	}

	if (bDoneClimbing)
	{
		bDoneClimbing = false;
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
