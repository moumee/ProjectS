// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TargetingSubsystem.h"

#include "ActorComponents/UISystem/ACTargetIndicator.h"

void UTargetingSubsystem::SetCurrentMissionTarget(AActor* NewTarget)
{
	// 1. 기존 타겟이 있었다면 인디케이터를 끈다.
	if (CurrentTarget.IsValid())
	{
		UACTargetIndicator* OldIndicatorComp = CurrentTarget->FindComponentByClass<UACTargetIndicator>();
		if (OldIndicatorComp)
		{
			OldIndicatorComp->HideIndicator();
		}
	}

	// 2. 새로운 타겟을 저장한다.
	CurrentTarget = NewTarget;

	// 3. 새로운 타겟이 유효하다면 인디케이터를 켠다.
	if (CurrentTarget.IsValid())
	{
		UACTargetIndicator* NewIndicatorComp = CurrentTarget->FindComponentByClass<UACTargetIndicator>();
		if (NewIndicatorComp)
		{
			NewIndicatorComp->ShowIndicator();
		}
		else
		{
			// 새 타겟에 인디케이터 컴포넌트가 없는 경우 경고 로그 출력
			UE_LOG(LogTemp, Warning, TEXT("TargetingSubsystem: New target '%s' does not have a UACTargetIndicator."), *NewTarget->GetName());
		}
	}
}

void UTargetingSubsystem::ClearCurrentMissionTarget()
{
	SetCurrentMissionTarget(nullptr);
}