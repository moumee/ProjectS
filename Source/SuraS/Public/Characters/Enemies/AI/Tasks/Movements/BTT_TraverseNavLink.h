// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_TraverseNavLink.generated.h"

/**
 * 
 */
UCLASS()
class SURAS_API UBTT_TraverseNavLink : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTT_TraverseNavLink();

protected:
	// 태스크가 실행될 때 호출되는 메인 함수
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	// 이동이 완료되었을 때 호출될 콜백 함수
	UFUNCTION()
	void OnTraversalCompleted(UBehaviorTreeComponent* BehaviorTreeComponent);
};
