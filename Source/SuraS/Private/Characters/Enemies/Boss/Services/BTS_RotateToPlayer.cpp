// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/Boss/Services/BTS_RotateToPlayer.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UBTS_RotateToPlayer::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);
	YawRateDegPerSec = 0.f;
}

void UBTS_RotateToPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if (AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(GetSelectedBlackboardKey())))
	{
		AAIController* AIController = OwnerComp.GetAIOwner();
		if (!AIController) return;
		FRotator CurrentRotation = AIController->GetPawn()->GetActorRotation();
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(AIController->GetPawn()->GetActorLocation(),
			TargetActor->GetActorLocation());

		float DeltaYaw = FMath::FindDeltaAngleDegrees(CurrentRotation.Yaw, LookAtRotation.Yaw);
		float DesiredRate = FMath::GetMappedRangeValueClamped(FVector2D(-180.f, 180.f),
			FVector2D(-MaxRotationRate, MaxRotationRate), DeltaYaw);
		YawRateDegPerSec = FMath::FInterpTo(YawRateDegPerSec, DesiredRate, DeltaSeconds, 15.f);
		float Step = YawRateDegPerSec * DeltaSeconds;
		float NewYaw = CurrentRotation.Yaw + Step;
		AIController->GetPawn()->SetActorRotation(FRotator(0.f, NewYaw, 0.0f));
	}
	
}
