// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/AI/Tasks/Movements/BTT_Climb.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemies/SuraCharacterEnemyBase.h"
#include "Characters/Enemies/AI/EnemyBaseAIController.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTT_Climb::UBTT_Climb(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Climb";
	bNotifyTick = true;
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTT_Climb::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	bDoneClimbing = false;
	
	if (ASuraCharacterEnemyBase* const Enemy = Cast<ASuraCharacterEnemyBase>(OwnerComp.GetAIOwner()->GetCharacter()))
	{
		// Enemy->GetAIController()->ClearFocus(EAIFocusPriority::Gameplay);

		CachedEnemy = Enemy;
		
		CachedEnemy->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		
		TargetLocation = CachedEnemy->GetActorLocation();
		Destination = OwnerComp.GetBlackboardComponent()->GetValueAsVector("TargetLocation");

		FRotator Rotation = OwnerComp.GetBlackboardComponent()->GetValueAsRotator("TargetRotation");
		CachedEnemy->SetActorRotation(Rotation);
		
		FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
		return EBTNodeResult::InProgress;
	}
	
	return EBTNodeResult::Failed;
}

void UBTT_Climb::TraceGroundAndWall()
{
	float EnemyHalfHeight = 0.f;
	
	EnemyHalfHeight = CachedEnemy->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	// Trace to the ground in the front
	FHitResult HitRightAhead;

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(CachedEnemy);

	FVector Start = CachedEnemy->GetActorLocation();
	FVector MovementDirection = CachedEnemy->GetActorForwardVector().GetSafeNormal();
	FVector End = Start + (MovementDirection * 500.f + CachedEnemy->GetActorUpVector() * -500.f).GetSafeNormal() * 200.f;

	bool bIsGroundRightAhead = GetWorld()->LineTraceSingleByChannel(
		HitRightAhead,
		Start,
		End,
		ECollisionChannel::ECC_Visibility,
		CollisionQueryParams
	);

	if (bIsGroundRightAhead)
	{
		// DrawDebugLine(GetWorld(), Start, HitRightAhead.Location, FColor::Red);
		
		FHitResult HitFurtherAhead;
		
		End = Start + (MovementDirection * 600.f + CachedEnemy->GetActorUpVector() * -500.f).GetSafeNormal() * 200.f;
		
		bool bIsGroundFurtherAhead = GetWorld()->LineTraceSingleByChannel(
			HitFurtherAhead,
			Start,
			End,
			ECollisionChannel::ECC_Visibility,
			CollisionQueryParams
		);

		if (bIsGroundFurtherAhead)
		{
			// DrawDebugLine(GetWorld(), Start, HitFurtherAhead.Location, FColor::Red);
			
			TargetLocation = HitRightAhead.Location + EnemyHalfHeight * HitRightAhead.Normal;

			FVector ActorForwardDirection = (HitFurtherAhead.Location - HitRightAhead.Location).GetSafeNormal();

			if (ActorForwardDirection.IsNearlyZero())
			{
				ActorForwardDirection = CachedEnemy->GetActorForwardVector().ProjectOnTo(HitRightAhead.Normal).GetSafeNormal();

				if (ActorForwardDirection.IsNearlyZero())
					ActorForwardDirection = FVector::ForwardVector.ProjectOnTo(HitRightAhead.Normal).GetSafeNormal();
			}
			
			FMatrix RotationMatrix(
				ActorForwardDirection,
				FVector::CrossProduct(HitRightAhead.Normal, ActorForwardDirection),
				HitRightAhead.Normal,
				FVector::ZeroVector
			);

			TargetRotation = FRotator(90.f, RotationMatrix.Rotator().Yaw, RotationMatrix.Rotator().Roll);

			// UE_LOG(LogTemp, Error, TEXT("Enemy Rotation %f, %f, %f"), TargetRotation.Pitch, TargetRotation.Yaw, TargetRotation.Roll);
		}
		else
			TraceLedge(EnemyHalfHeight);
	}
	else // hit the top of the wall so that the surface is curved out; previous hit detection can only detect an empty space of air
		TraceLedge(EnemyHalfHeight);
}

void UBTT_Climb::TraceLedge(const float EnemyHalfHeight)
{
	// bHasReachedLedge = true;
	
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(CachedEnemy);
	
	FVector MovementDirection = CachedEnemy->GetActorForwardVector().GetSafeNormal();
	FVector LedgeStartGap = (MovementDirection * 500.f + CachedEnemy->GetActorUpVector() * -500.f).GetSafeNormal();
	FVector Start = CachedEnemy->GetActorLocation() + LedgeStartGap * 100.f;
	FVector End = Start + LedgeStartGap.RotateAngleAxis(90.f, CachedEnemy->GetActorRightVector()) * 200.f;
	
	FHitResult HitWallLedge;
		
	bool bIsOnLedge = GetWorld()->LineTraceSingleByChannel(
		HitWallLedge,
		Start,
		End,
		ECollisionChannel::ECC_Visibility,
		CollisionQueryParams
	);

	if (bIsOnLedge)
	{
		// DrawDebugLine(GetWorld(), Start, HitWallLedge.Location, FColor::Red);

		FHitResult HitFurtherWallLedge;

		End = Start + LedgeStartGap.RotateAngleAxis(85.f, CachedEnemy->GetActorRightVector()) * 200.f;
			
		bool bIsOnFurtherLedge = GetWorld()->LineTraceSingleByChannel(
			HitFurtherWallLedge,
			Start,
			End,
			ECollisionChannel::ECC_Visibility,
			CollisionQueryParams
		);

		if (bIsOnFurtherLedge)
		{
			// DrawDebugLine(GetWorld(), Start, HitFurtherWallLedge.Location, FColor::Red);

			FHitResult HitWallLedgeSide;

			End = Start + LedgeStartGap.RotateAngleAxis(90.f, CachedEnemy->GetActorRightVector()).RotateAngleAxis(5.f, LedgeStartGap) * 200.f;

			bool bIsOnWallLedgeSide = GetWorld()->LineTraceSingleByChannel(
				HitWallLedgeSide,
				Start,
				End,
				ECollisionChannel::ECC_Visibility,
				CollisionQueryParams
			);

			if (bIsOnWallLedgeSide)
			{
				TargetLocation = HitWallLedge.Location + EnemyHalfHeight * HitWallLedge.Normal;
				FMatrix RotationMatrix(
					(HitFurtherWallLedge.Location - HitWallLedge.Location).GetSafeNormal(),
					(HitWallLedgeSide.Location - HitWallLedge.Location).GetSafeNormal(),
					HitWallLedge.Normal,
					FVector::ZeroVector
				);
				TargetRotation = RotationMatrix.Rotator();
			}
			else
				bDoneClimbing = true;
		}
		else
			bDoneClimbing = true;
	}
	else
		bDoneClimbing = true;
}

void UBTT_Climb::Move(UBehaviorTreeComponent& OwnerComp) const
{
	if (!CachedEnemy)
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	
	CachedEnemy->SetActorRotation(FMath::RInterpTo(CachedEnemy->GetActorRotation(), TargetRotation, GetWorld()->GetDeltaSeconds(), 15.f));
	
	if (FVector::Dist(TargetLocation, CachedEnemy->GetActorLocation()) > 2.f)
		CachedEnemy->AddMovementInput((TargetLocation - CachedEnemy->GetActorLocation()).GetSafeNormal());
}

void UBTT_Climb::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	
	TraceGroundAndWall();
	Move(OwnerComp);

	// UE_LOG(LogTemp, Error, TEXT("Climbing Destination Dist: %f"), FVector::Distance(CachedEnemy->GetActorLocation(), Destination));

	if (FVector::Distance(CachedEnemy->GetActorLocation(), Destination) < ArrivalAcceptance)
	{
		// UE_LOG(LogTemp, Error, TEXT("Climbing Destination"));
		bDoneClimbing = true;
	}

	if (bDoneClimbing)
	{
		// UE_LOG(LogTemp, Error, TEXT("Climbing Done"));
		
		ASuraPawnPlayer* const Player = Cast<ASuraPawnPlayer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("AttackTarget"));

		// CachedEnemy->SetActorLocation(Destination);
		CachedEnemy->SetActorRotation(OwnerComp.GetBlackboardComponent()->GetValueAsRotator("TargetRotation"));
		CachedEnemy->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		
		CachedEnemy->GetAIController()->SetFocus(Player, EAIFocusPriority::Gameplay);
		CachedEnemy->GetAIController()->SetStateToChaseOrPursue(Player);
		
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
