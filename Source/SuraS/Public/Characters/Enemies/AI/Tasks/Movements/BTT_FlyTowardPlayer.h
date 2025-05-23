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
	// ������
	UBTT_FlyTowardPlayer();

	// Behavior Tree ���� �� ȣ��Ǵ� �Լ� (�ʼ�)
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	// �� �����Ӹ��� ȣ��Ǵ� �Լ� (���� ����, �������� �����ӿ� ����)
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	// ������ �÷��̾ ������ ����
	UPROPERTY(EditDefaultsOnly, Category = "Target")
	TSubclassOf<APlayerController> PlayerControllerClass;

	// ���� �ӵ�
	UPROPERTY(EditDefaultsOnly, Category = "Movement", meta = (ClampMin = "0.0"))
	float FlySpeed;

private:
	// ã�� �÷��̾� ��Ʈ�ѷ��� ������ ����
	APlayerController* TargetPlayerController;
};
