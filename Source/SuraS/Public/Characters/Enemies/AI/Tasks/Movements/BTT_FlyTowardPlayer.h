// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_FlyTowardPlayer.generated.h"

/**
 * 
 */
UCLASS()
class SURAS_API UBTT_FlyTowardPlayer : public UBTTaskNode
{
	GENERATED_BODY()

public:
	// 생성자
	UBTT_FlyTowardPlayer();

	// Behavior Tree 실행 시 호출되는 함수 (필수)
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	// 매 프레임마다 호출되는 함수 (선택 사항, 지속적인 움직임에 유용)
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	// 추적할 플레이어를 저장할 변수
	UPROPERTY(EditDefaultsOnly, Category = "Target")
	TSubclassOf<APlayerController> PlayerControllerClass;

	// 비행 속도
	UPROPERTY(EditDefaultsOnly, Category = "Movement", meta = (ClampMin = "0.0"))
	float FlySpeed;

private:
	// 찾은 플레이어 컨트롤러를 저장할 변수
	APlayerController* TargetPlayerController;
};
