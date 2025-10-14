// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "TargetingSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class SURAS_API UTargetingSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// 새로운 미션 목표를 설정합니다. 블루프린트에서도 호출 가능하도록 합니다.
	UFUNCTION(BlueprintCallable, Category = "Targeting")
	void SetCurrentMissionTarget(AActor* NewTarget);

	// 현재 설정된 미션 목표를 해제합니다.
	UFUNCTION(BlueprintCallable, Category = "Targeting")
	void ClearCurrentMissionTarget();

private:
	// 현재 활성화된 목표 액터입니다.
	TWeakObjectPtr<AActor> CurrentTarget;
	
};
