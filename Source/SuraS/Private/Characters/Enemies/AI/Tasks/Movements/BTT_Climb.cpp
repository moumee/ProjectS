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
}

EBTNodeResult::Type UBTT_Climb::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	bDoneClimbing = false;
	
	if (ASuraCharacterEnemyBase* const Enemy = Cast<ASuraCharacterEnemyBase>(OwnerComp.GetAIOwner()->GetCharacter()))
	{
		Enemy->GetAIController()->ClearFocus(EAIFocusPriority::Gameplay);
		
		Enemy->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
		
		TargetLocation = Enemy->GetActorLocation();
		
		FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
		return EBTNodeResult::InProgress;
	}
	
	return EBTNodeResult::Failed;
}

void UBTT_Climb::TraceGroundAndWall(AActor* OwningActor)
{
	float EnemyHalfHeight = 0.f;

	if (const UCapsuleComponent* CapsuleComp = OwningActor->FindComponentByClass<UCapsuleComponent>())
	{
		EnemyHalfHeight = CapsuleComp->GetScaledCapsuleHalfHeight();
	}
	else // if no capsule component, but I doubt
	{
		EnemyHalfHeight = 90.0f; // Typical enemy character half-height
	}

	// Trace to the ground in the front
	FHitResult HitRightAhead;

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(OwningActor);

	FVector Start = OwningActor->GetActorLocation();
	FVector MovementDirection = OwningActor->GetActorForwardVector().GetSafeNormal();
	FVector End = Start + (MovementDirection * 500.f + OwningActor->GetActorUpVector() * -500.f).GetSafeNormal() * 200.f;

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
		
		End = Start + (MovementDirection * 600.f + OwningActor->GetActorUpVector() * -500.f).GetSafeNormal() * 200.f;
		
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
			
			End = Start + MovementDirection * 500.f + OwningActor->GetActorUpVector() * -500.f + MovementDirection.RotateAngleAxis(90.f, OwningActor->GetActorUpVector()) * 100.f;

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

				TargetLocation = HitRightAhead.Location + EnemyHalfHeight * HitRightAhead.Normal;
				
				FMatrix RotationMatrix(
					(HitFurtherAhead.Location - HitRightAhead.Location).GetSafeNormal(),
					(HitRightAheadSide.Location - HitRightAhead.Location).GetSafeNormal(),
					HitRightAhead.Normal,
					FVector::ZeroVector
				);

				TargetRotation = RotationMatrix.Rotator();

				/*FVector TempForwardAlongWall = OwningActor->GetActorForwardVector().ProjectOnTo(HitRightAhead.Normal);
				TempForwardAlongWall.Normalize();
				if (TempForwardAlongWall.IsNearlyZero())
				{
					// Fallback if current forward is parallel to wall normal
					TempForwardAlongWall = FVector::CrossProduct(HitRightAhead.Normal, FVector::UpVector).GetSafeNormal();
					if (TempForwardAlongWall.IsNearlyZero())
					{
						TempForwardAlongWall = FVector::CrossProduct(HitRightAhead.Normal, OwningActor->GetActorRightVector()).GetSafeNormal();
					}
				}

				TargetRotation  = FRotationMatrix::MakeFromXZ(TempForwardAlongWall, HitRightAhead.Normal).Rotator();*/
			}
			else
				TraceLedge(OwningActor, EnemyHalfHeight);
		}
		else
			TraceLedge(OwningActor, EnemyHalfHeight);
	}
	else // hit the top of the wall so that the surface is curved out; previous hit detection can only detect an empty space of air
		TraceLedge(OwningActor, EnemyHalfHeight);
}

void UBTT_Climb::TraceLedge(AActor* OwningActor, float EnemyHalfHeight)
{
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(OwningActor);
	
	FVector MovementDirection = OwningActor->GetActorForwardVector().GetSafeNormal();
	FVector LedgeStartGap = (MovementDirection * 500.f + OwningActor->GetActorUpVector() * -500.f).GetSafeNormal();
	FVector Start = OwningActor->GetActorLocation() + LedgeStartGap * 100.f;
	FVector End = Start + LedgeStartGap.RotateAngleAxis(90.f, OwningActor->GetActorRightVector()) * 200.f;
	
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
		DrawDebugLine(GetWorld(), Start, HitWallLedge.Location, FColor::Red);

		FHitResult HitFurtherWallLedge;

		End = Start + LedgeStartGap.RotateAngleAxis(85.f, OwningActor->GetActorRightVector()) * 200.f;
			
		bool bIsOnFurtherLedge = GetWorld()->LineTraceSingleByChannel(
			HitFurtherWallLedge,
			Start,
			End,
			ECollisionChannel::ECC_Visibility,
			CollisionQueryParams
		);

		if (bIsOnFurtherLedge)
		{
			DrawDebugLine(GetWorld(), Start, HitFurtherWallLedge.Location, FColor::Red);

			FHitResult HitWallLedgeSide;

			End = Start + LedgeStartGap.RotateAngleAxis(90.f, OwningActor->GetActorRightVector()).RotateAngleAxis(5.f, LedgeStartGap) * 200.f;

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
	ACharacter* Character = Cast<ACharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (!Character)
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	
	Character->SetActorRotation(FMath::RInterpTo(Character->GetActorRotation(), TargetRotation, GetWorld()->GetDeltaSeconds(), 15.f));
	
	if (FVector::Dist(TargetLocation, Character->GetActorLocation()) > 2.f)
	{
		Character->AddMovementInput((TargetLocation - Character->GetActorLocation()).GetSafeNormal());
	}
}

void UBTT_Climb::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AActor* OwningEnemy = OwnerComp.GetAIOwner()->GetCharacter();
	ASuraPawnPlayer* const Player = Cast<ASuraPawnPlayer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("AttackTarget"));

	TraceGroundAndWall(OwningEnemy);
	Move(OwnerComp);

	if (bDoneClimbing)
	{
		bDoneClimbing = false;
		
		if (ASuraCharacterEnemyBase* const Enemy = Cast<ASuraCharacterEnemyBase>(OwnerComp.GetAIOwner()->GetCharacter()))
		{
			Enemy->GetAIController()->SetFocus(Player, EAIFocusPriority::Gameplay);
			Enemy->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		}
		
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
