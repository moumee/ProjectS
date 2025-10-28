// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/Boss/Tasks/BTT_RotateToTarget.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"

UBTT_RotateToTarget::UBTT_RotateToTarget()
{
	NodeName = "Rotate To Target";

	INIT_TASK_NODE_NOTIFY_FLAGS();
}

EBTNodeResult::Type UBTT_RotateToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UObject* TargetObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetKey.SelectedKeyName);
	if (!TargetObject) return EBTNodeResult::Failed;
	AActor* TargetActor = Cast<AActor>(TargetObject);
	APawn* OwningPawn = OwnerComp.GetAIOwner()->GetPawn();

	FRotateToTargetMemory* Memory = CastInstanceNodeMemory<FRotateToTargetMemory>(NodeMemory);
	check(Memory);
	Memory->TargetActor = TargetActor;
	Memory->OwningPawn = OwningPawn;
	Memory->YawRateDegPerSec = 0.f;

	if (!Memory->IsValid()) return EBTNodeResult::Failed;

	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(OwningPawn->GetActorLocation(),
		TargetActor->GetActorLocation());
	
	if (FMath::Abs(FMath::FindDeltaAngleDegrees(OwningPawn->GetActorRotation().Yaw, LookAtRotation.Yaw)) < 2.f)
	{
		Memory->Reset();
		Memory->YawRateDegPerSec = 0.f;
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::InProgress;
}

void UBTT_RotateToTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FRotateToTargetMemory* Memory = CastInstanceNodeMemory<FRotateToTargetMemory>(NodeMemory);

	APawn* OwningPawn = Memory->OwningPawn.Get();
	AActor* TargetActor = Memory->TargetActor.Get();

	if (!OwningPawn || !TargetActor) FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(OwningPawn->GetActorLocation(),
		TargetActor->GetActorLocation());
	float DeltaYaw = FMath::FindDeltaAngleDegrees(OwningPawn->GetActorRotation().Yaw, LookAtRotation.Yaw);
	if (FMath::Abs(DeltaYaw) < 2.f)
	{
		Memory->Reset();
		Memory->YawRateDegPerSec = 0.f;
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	else
	{
		float DesiredRate = FMath::GetMappedRangeValueClamped(FVector2D(-180.f, 180.f),
		FVector2D(-MaxYawRate, MaxYawRate), DeltaYaw);
		Memory->YawRateDegPerSec = FMath::FInterpTo(Memory->YawRateDegPerSec, DesiredRate, DeltaSeconds, 15.f);
		float Step = Memory->YawRateDegPerSec * DeltaSeconds;
		float NewYaw = OwningPawn->GetActorRotation().Yaw + Step;
		OwningPawn->SetActorRotation(FRotator(0.f, NewYaw, 0.0f));
	}
}

uint16 UBTT_RotateToTarget::GetInstanceMemorySize() const
{
	return sizeof(FRotateToTargetMemory);
}

void UBTT_RotateToTarget::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	if (UBlackboardData* BBAsset = GetBlackboardAsset())
	{
		TargetKey.ResolveSelectedKey(*BBAsset);
	}
}
