// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/AI/Tasks/Movements/BTT_Climb.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemies/SuraCharacterEnemyBase.h"
#include "Characters/Enemies/AI/EnemyBaseAIController.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#define WALL_TRACE_CHANNEL ECC_GameTraceChannel2
#define ENEMY_TRACE_CHANNEL ECC_GameTraceChannel6

UBTT_Climb::UBTT_Climb(FObjectInitializer const& ObjectInitializer)
{
	NodeName = "Climb";
	INIT_TASK_NODE_NOTIFY_FLAGS();
}

EBTNodeResult::Type UBTT_Climb::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	check(Cast<ASuraCharacterEnemyBase>(OwnerComp.GetAIOwner()->GetCharacter()))

	FBTTClimbTaskMemory* Mem = CastInstanceNodeMemory<FBTTClimbTaskMemory>(NodeMemory);
	check(Mem);
	
	if (ASuraCharacterEnemyBase* Enemy = Cast<ASuraCharacterEnemyBase>(OwnerComp.GetAIOwner()->GetCharacter()))
	{
		// Enemy->GetAIController()->ClearFocus(EAIFocusPriority::Gameplay);

		Mem->CachedEnemy = Enemy;

		UAnimMontage* ClimbAnimation = Enemy->GetClimbMontage();
		Enemy->PlayAnimMontage(ClimbAnimation);
		
		Enemy->GetCharacterMovement()->SetMovementMode(MOVE_Flying);

		Mem->bHasLedgeDetected = false;
		Mem->bIsDoneClimbing = false;
		
		FRotator Rotation = OwnerComp.GetBlackboardComponent()->GetValueAsRotator("TargetRotation");
		Enemy->SetActorRotation(Rotation);
		
		FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
		return EBTNodeResult::InProgress;
	}
	
	return EBTNodeResult::Failed;
}

void UBTT_Climb::TraceGroundAndWall(uint8* NodeMemory)
{
	FBTTClimbTaskMemory* Mem = CastInstanceNodeMemory<FBTTClimbTaskMemory>(NodeMemory);
	
	float EnemyHalfHeight = Mem->CachedEnemy.Get()->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	// Trace to the ledge above
	FHitResult WallHit;
	FCollisionQueryParams ClimbLedgeParams;
	FCollisionResponseParams ClimbResponseParams;
	ClimbLedgeParams.AddIgnoredActor(Mem->CachedEnemy.Get());
	ClimbResponseParams.CollisionResponse.SetResponse(ENEMY_TRACE_CHANNEL, ECR_Ignore);
	FVector ClimbLedgeSweepEnd = Mem->CachedEnemy.Get()->GetActorLocation() + Mem->CachedEnemy.Get()->GetActorForwardVector() * Mem->CachedEnemy.Get()->GetCapsuleComponent()->GetCollisionShape().GetCapsuleRadius();

	bool bIsWallAhead = GetWorld()->SweepSingleByChannel(
		WallHit,
		Mem->CachedEnemy.Get()->GetActorLocation(),
		ClimbLedgeSweepEnd,
		Mem->CachedEnemy.Get()->GetActorQuat(),
		WALL_TRACE_CHANNEL,
		Mem->CachedEnemy.Get()->GetCapsuleComponent()->GetCollisionShape(),
		ClimbLedgeParams,
		ClimbResponseParams
	);

	// DrawDebugDirectionalArrow(GetWorld(), CachedEnemy->GetActorLocation(), CachedEnemy->GetActorLocation() + TargetVelocity, 10.f, FColor::Red);

	if (bIsWallAhead && WallHit.bBlockingHit && WallHit.ImpactNormal.Z < FMath::Cos(FMath::DegreesToRadians(50.f)) && WallHit.ImpactNormal.Z > -0.3f) // MinWalkableFloorZ = FMath::Cos(FMath::DegreesToRadians(MaxWalkableFloorAngle))
	{
		// DrawDebugLine(GetWorld(), Start, HitRightAhead.Location, FColor::Red);

		FVector LedgeHitStart = FVector(WallHit.ImpactPoint.X, WallHit.ImpactPoint.Y, Mem->CachedEnemy.Get()->GetActorLocation().Z + 100.f);
		FVector LedgeHitEnd = LedgeHitStart - FVector(0, 0, EnemyHalfHeight + 110.f);
		
		bool bLedgeFloorHit = GetWorld()->SweepSingleByChannel(
			WallHit,
			LedgeHitStart,
			LedgeHitEnd,
			Mem->CachedEnemy.Get()->GetActorQuat(),
			ECC_WorldStatic,
			Mem->CachedEnemy.Get()->GetCapsuleComponent()->GetCollisionShape(),
			ClimbLedgeParams,
			ClimbResponseParams
		);

		// DrawDebugCapsuleTraceSingle(GetWorld(), LedgeHitStart, LedgeHitEnd, CachedEnemy->GetCapsuleComponent()->GetCollisionShape().GetCapsuleRadius(), CachedEnemy->GetCapsuleComponent()->GetCollisionShape().GetCapsuleHalfHeight(), EDrawDebugTrace::ForDuration, bLedgeFloorHit && WallHit.IsValidBlockingHit(), WallHit, FLinearColor::Red, FLinearColor::Green, 1.f);
		
		if (bLedgeFloorHit && WallHit.IsValidBlockingHit() && WallHit.ImpactNormal.Z >= FMath::Cos(FMath::DegreesToRadians(50.f)))
		{
			// UE_LOG(LogTemp, Warning, TEXT("climb: MoveUpTheLedge"));
			Mem->bHasLedgeDetected = true;
			MoveUpTheLedge(NodeMemory, WallHit.ImpactNormal);
		}
		else
		{
			// UE_LOG(LogTemp, Warning, TEXT("climb: no ledge yet"));
			Mem->TargetVelocity = Mem->CachedEnemy.Get()->GetActorUpVector() * 600.f; // has not reached ledge yet
		}

		FMatrix RotationMatrix(
			 Mem->CachedEnemy.Get()->GetMesh()->GetForwardVector(),
			FVector::CrossProduct(WallHit.Normal, Mem->CachedEnemy.Get()->GetMesh()->GetForwardVector()),
			WallHit.Normal,
			FVector::ZeroVector
		);

		Mem->TargetRotation = FRotator(RotationMatrix.Rotator().Pitch, -90, RotationMatrix.Rotator().Roll);
	}
	else
	{
		// UE_LOG(LogTemp, Warning, TEXT("climb: Forward vector"));
		
		if (!Mem->bHasLedgeDetected)
		{
			Mem->TargetVelocity = Mem->CachedEnemy.Get()->GetActorForwardVector() * 600.f; // has not reached wall yet
		}
		else // time to walk on the ledge floor if ledge has been climbed up
		{
			// UE_LOG(LogTemp, Error, TEXT("IS IT ME"))

			FHitResult GroundHit;
			bool bIsGroundBelow = GetWorld()->LineTraceSingleByChannel(
				GroundHit,
				Mem->CachedEnemy.Get()->GetActorLocation(),
				Mem->CachedEnemy.Get()->GetActorLocation() + FVector::DownVector * 500,
				WALL_TRACE_CHANNEL,
				ClimbLedgeParams,
				ClimbResponseParams
			);

			if (bIsGroundBelow)
			{
				// DrawDebugLine(GetWorld(), CachedEnemy->GetActorLocation(), GroundHit.Location, FColor::Red, true);
				Mem->bIsDoneClimbing = true;
			}
			else
			{
				Mem->TargetVelocity = Mem->CachedEnemy.Get()->GetActorUpVector() * 600.f;
				Mem->TargetVelocity += Mem->CachedEnemy.Get()->GetActorForwardVector() * 600.f;
			}
		}
	}
}

void UBTT_Climb::MoveUpTheLedge(uint8* NodeMemory, FVector ImpactNormal)
{
	FBTTClimbTaskMemory* Mem = CastInstanceNodeMemory<FBTTClimbTaskMemory>(NodeMemory);
	
	FVector LedgeFloorSlope = FVector::VectorPlaneProject(Mem->CachedEnemy.Get()->GetActorForwardVector(), ImpactNormal).GetSafeNormal();

	FHitResult LedgeHit;
	FCollisionQueryParams LedgeParams;
	FCollisionResponseParams LedgeResponseParams;
	LedgeParams.AddIgnoredActor(Mem->CachedEnemy.Get());
	LedgeResponseParams.CollisionResponse.SetResponse(ENEMY_TRACE_CHANNEL, ECR_Ignore);
	FVector LedgeSweepEnd = Mem->CachedEnemy.Get()->GetActorLocation() + LedgeFloorSlope * Mem->CachedEnemy.Get()->GetCapsuleComponent()->GetCollisionShape().GetCapsuleRadius();

	bool bLedgeHit = GetWorld()->SweepSingleByChannel(
		LedgeHit,
		Mem->CachedEnemy.Get()->GetActorLocation(),
		LedgeSweepEnd,
		Mem->CachedEnemy.Get()->GetActorQuat(),
		WALL_TRACE_CHANNEL,
		Mem->CachedEnemy.Get()->GetCapsuleComponent()->GetCollisionShape(),
		LedgeParams,
		LedgeResponseParams
	);

	if (!bLedgeHit)
	{
		// UE_LOG(LogTemp, Error, TEXT("LEDGE REACHED"))
		Mem->TargetVelocity = LedgeFloorSlope * 600.f;
	}
	else
	{
		Mem->TargetVelocity = FVector::VectorPlaneProject(FVector::UpVector, LedgeHit.ImpactNormal).GetSafeNormal() * 600.f;
		// UE_LOG(LogTemp, Warning, TEXT("climb: prj vector %f, %f, %f"), TargetVelocity.X, TargetVelocity.Y, TargetVelocity.Z);
	}
}

void UBTT_Climb::Move(uint8* NodeMemory, UBehaviorTreeComponent& OwnerComp) const
{
	FBTTClimbTaskMemory* Mem = CastInstanceNodeMemory<FBTTClimbTaskMemory>(NodeMemory);
	
	if (!Mem->CachedEnemy.IsValid())
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	
	Mem->CachedEnemy.Get()->GetMesh()->SetRelativeRotation(FMath::RInterpTo(Mem->CachedEnemy.Get()->GetMesh()->GetRelativeRotation(), Mem->TargetRotation, GetWorld()->GetDeltaSeconds(), 15.f));

	Mem->CachedEnemy.Get()->GetCharacterMovement()->Velocity = Mem->TargetVelocity;
}

uint16 UBTT_Climb::GetInstanceMemorySize() const
{
	return sizeof(FBTTClimbTaskMemory);
}

void UBTT_Climb::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	FBTTClimbTaskMemory* Mem = CastInstanceNodeMemory<FBTTClimbTaskMemory>(NodeMemory);
	
	TraceGroundAndWall(NodeMemory);
	Move(NodeMemory, OwnerComp);

	if (Mem->bIsDoneClimbing)
	{
		Mem->CachedEnemy.Get()->StopAnimMontage();
		Mem->CachedEnemy.Get()->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		Mem->TargetRotation = Mem->CachedEnemy.Get()->GetAIController()->GetBlackboardComponent()->GetValueAsRotator("TargetRotation");
		Mem->CachedEnemy.Get()->SetActorRotation(Mem->CachedEnemy.Get()->GetAIController()->GetBlackboardComponent()->GetValueAsRotator("TargetRotation"));
		Mem->CachedEnemy.Get()->GetAIController()->SetStateToChaseOrPursue(Mem->CachedEnemy.Get()->GetAIController()->GetAttackTarget());
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
