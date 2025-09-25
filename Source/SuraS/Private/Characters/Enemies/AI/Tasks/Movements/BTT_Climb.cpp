// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/AI/Tasks/Movements/BTT_Climb.h"

#include "KismetTraceUtils.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemies/SuraCharacterEnemyBase.h"
#include "Characters/Enemies/AI/EnemyBaseAIController.h"
#include "Components/CapsuleComponent.h"
#include "Concepts/Iterable.h"
#include "Evaluation/IMovieSceneEvaluationHook.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Widgets/Text/STextScroller.h"

#define WALL_TRACE_CHANNEL ECC_GameTraceChannel2
#define ENEMY_TRACE_CHANNEL ECC_GameTraceChannel6

UBTT_Climb::UBTT_Climb(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Climb";
	bNotifyTick = true;
	bCreateNodeInstance = true;
}

EBTNodeResult::Type UBTT_Climb::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	check(Cast<ASuraCharacterEnemyBase>(OwnerComp.GetAIOwner()->GetCharacter()))
	
	if (ASuraCharacterEnemyBase* Enemy = Cast<ASuraCharacterEnemyBase>(OwnerComp.GetAIOwner()->GetCharacter()))
	{
		// Enemy->GetAIController()->ClearFocus(EAIFocusPriority::Gameplay);

		CachedEnemy = Enemy;

		UAnimMontage* ClimbAnimation = CachedEnemy->GetClimbMontage();
		CachedEnemy->PlayAnimMontage(ClimbAnimation);
		
		CachedEnemy->GetCharacterMovement()->SetMovementMode(MOVE_Flying);

		bHasLedgeDetected = false;
		bIsDoneClimbing = false;
		
		FRotator Rotation = OwnerComp.GetBlackboardComponent()->GetValueAsRotator("TargetRotation");
		CachedEnemy->SetActorRotation(Rotation);
		
		FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
		return EBTNodeResult::InProgress;
	}
	
	return EBTNodeResult::Failed;
}

void UBTT_Climb::TraceGroundAndWall()
{
	float EnemyHalfHeight = CachedEnemy->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	// Trace to the ledge above
	FHitResult WallHit;
	FCollisionQueryParams ClimbLedgeParams;
	FCollisionResponseParams ClimbResponseParams;
	ClimbLedgeParams.AddIgnoredActor(CachedEnemy.Get());
	ClimbResponseParams.CollisionResponse.SetResponse(ENEMY_TRACE_CHANNEL, ECR_Ignore);
	FVector ClimbLedgeSweepEnd = CachedEnemy->GetActorLocation() + CachedEnemy->GetActorForwardVector() * CachedEnemy->GetCapsuleComponent()->GetCollisionShape().GetCapsuleRadius();

	bool bIsWallAhead = GetWorld()->SweepSingleByChannel(
		WallHit,
		CachedEnemy->GetActorLocation(),
		ClimbLedgeSweepEnd,
		CachedEnemy->GetActorQuat(),
		WALL_TRACE_CHANNEL,
		CachedEnemy->GetCapsuleComponent()->GetCollisionShape(),
		ClimbLedgeParams,
		ClimbResponseParams
	);

	// DrawDebugDirectionalArrow(GetWorld(), CachedEnemy->GetActorLocation(), CachedEnemy->GetActorLocation() + TargetVelocity, 10.f, FColor::Red);

	if (bIsWallAhead && WallHit.bBlockingHit && WallHit.ImpactNormal.Z < FMath::Cos(FMath::DegreesToRadians(50.f)) && WallHit.ImpactNormal.Z > -0.3f) // MinWalkableFloorZ = FMath::Cos(FMath::DegreesToRadians(MaxWalkableFloorAngle))
	{
		// DrawDebugLine(GetWorld(), Start, HitRightAhead.Location, FColor::Red);

		FVector LedgeHitStart = FVector(WallHit.ImpactPoint.X, WallHit.ImpactPoint.Y, CachedEnemy->GetActorLocation().Z + 100.f);
		FVector LedgeHitEnd = LedgeHitStart - FVector(0, 0, EnemyHalfHeight + 110.f);
		
		bool bLedgeFloorHit = GetWorld()->SweepSingleByChannel(
			WallHit,
			LedgeHitStart,
			LedgeHitEnd,
			CachedEnemy->GetActorQuat(),
			ECC_WorldStatic,
			CachedEnemy->GetCapsuleComponent()->GetCollisionShape(),
			ClimbLedgeParams,
			ClimbResponseParams
		);

		// DrawDebugCapsuleTraceSingle(GetWorld(), LedgeHitStart, LedgeHitEnd, CachedEnemy->GetCapsuleComponent()->GetCollisionShape().GetCapsuleRadius(), CachedEnemy->GetCapsuleComponent()->GetCollisionShape().GetCapsuleHalfHeight(), EDrawDebugTrace::ForDuration, bLedgeFloorHit && WallHit.IsValidBlockingHit(), WallHit, FLinearColor::Red, FLinearColor::Green, 1.f);
		
		if (bLedgeFloorHit && WallHit.IsValidBlockingHit() && WallHit.ImpactNormal.Z >= FMath::Cos(FMath::DegreesToRadians(50.f)))
		{
			// UE_LOG(LogTemp, Warning, TEXT("climb: MoveUpTheLedge"));
			bHasLedgeDetected = true;
			MoveUpTheLedge(WallHit.ImpactNormal);
		}
		else
		{
			// UE_LOG(LogTemp, Warning, TEXT("climb: no ledge yet"));
			TargetVelocity = CachedEnemy->GetActorUpVector() * 600.f; // has not reached ledge yet
		}

		FMatrix RotationMatrix(
			 CachedEnemy->GetMesh()->GetForwardVector(),
			FVector::CrossProduct(WallHit.Normal, CachedEnemy->GetMesh()->GetForwardVector()),
			WallHit.Normal,
			FVector::ZeroVector
		);

		TargetRotation = FRotator(RotationMatrix.Rotator().Pitch, -90, RotationMatrix.Rotator().Roll);
	}
	else
	{
		// UE_LOG(LogTemp, Warning, TEXT("climb: Forward vector"));
		
		if (!bHasLedgeDetected)
		{
			TargetVelocity = CachedEnemy->GetActorForwardVector() * 600.f; // has not reached wall yet
		}
		else // time to walk on the ledge floor if ledge has been climbed up
		{
			// UE_LOG(LogTemp, Error, TEXT("IS IT ME"))

			FHitResult GroundHit;
			bool bIsGroundBelow = GetWorld()->LineTraceSingleByChannel(
				GroundHit,
				CachedEnemy->GetActorLocation(),
				CachedEnemy->GetActorLocation() + FVector::DownVector * 500,
				WALL_TRACE_CHANNEL,
				ClimbLedgeParams,
				ClimbResponseParams
			);

			if (bIsGroundBelow)
			{
				// DrawDebugLine(GetWorld(), CachedEnemy->GetActorLocation(), GroundHit.Location, FColor::Red, true);
				bIsDoneClimbing = true;
			}
			else
			{
				TargetVelocity = CachedEnemy->GetActorUpVector() * 600.f;
				TargetVelocity += CachedEnemy->GetActorForwardVector() * 600.f;
			}
		}
	}
}

void UBTT_Climb::MoveUpTheLedge(FVector ImpactNormal)
{
	FVector LedgeFloorSlope = FVector::VectorPlaneProject(CachedEnemy->GetActorForwardVector(), ImpactNormal).GetSafeNormal();

	FHitResult LedgeHit;
	FCollisionQueryParams LedgeParams;
	FCollisionResponseParams LedgeResponseParams;
	LedgeParams.AddIgnoredActor(CachedEnemy.Get());
	LedgeResponseParams.CollisionResponse.SetResponse(ENEMY_TRACE_CHANNEL, ECR_Ignore);
	FVector LedgeSweepEnd = CachedEnemy->GetActorLocation() + LedgeFloorSlope * CachedEnemy->GetCapsuleComponent()->GetCollisionShape().GetCapsuleRadius();

	bool bLedgeHit = GetWorld()->SweepSingleByChannel(
		LedgeHit,
		CachedEnemy->GetActorLocation(),
		LedgeSweepEnd,
		CachedEnemy->GetActorQuat(),
		WALL_TRACE_CHANNEL,
		CachedEnemy->GetCapsuleComponent()->GetCollisionShape(),
		LedgeParams,
		LedgeResponseParams
	);

	if (!bLedgeHit)
	{
		// UE_LOG(LogTemp, Error, TEXT("LEDGE REACHED"))
		TargetVelocity = LedgeFloorSlope * 600.f;
	}
	else
	{
		TargetVelocity = FVector::VectorPlaneProject(FVector::UpVector, LedgeHit.ImpactNormal).GetSafeNormal() * 600.f;
		// UE_LOG(LogTemp, Warning, TEXT("climb: prj vector %f, %f, %f"), TargetVelocity.X, TargetVelocity.Y, TargetVelocity.Z);
	}
}

void UBTT_Climb::Move(UBehaviorTreeComponent& OwnerComp) const
{
	if (!CachedEnemy.IsValid())
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	
	CachedEnemy->GetMesh()->SetRelativeRotation(FMath::RInterpTo(CachedEnemy->GetMesh()->GetRelativeRotation(), TargetRotation, GetWorld()->GetDeltaSeconds(), 15.f));

	CachedEnemy->GetCharacterMovement()->Velocity = TargetVelocity;
}

void UBTT_Climb::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	
	TraceGroundAndWall();
	Move(OwnerComp);

	if (bIsDoneClimbing)
	{
		CachedEnemy->StopAnimMontage();
		CachedEnemy->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		TargetRotation = CachedEnemy->GetAIController()->GetBlackboardComponent()->GetValueAsRotator("TargetRotation");
		CachedEnemy->SetActorRotation(CachedEnemy->GetAIController()->GetBlackboardComponent()->GetValueAsRotator("TargetRotation"));
		CachedEnemy->GetAIController()->SetStateToChaseOrPursue(CachedEnemy->GetAIController()->GetAttackTarget());
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
