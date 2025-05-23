// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/AI/Tasks/Movements/BTT_FlyTowardPlayer.h"
#include "AIController.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "Kismet/KismetMathLibrary.h"

UBTT_FlyTowardPlayer::UBTT_FlyTowardPlayer()
{
	NodeName = TEXT("Fly");
	// TickTask 활성화
	bNotifyTick = true;
	TargetPlayerController = nullptr;
	FlySpeed = 2.0f; // 기본 비행 속도 설정
}

EBTNodeResult::Type UBTT_FlyTowardPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}

	// PlayerControllerClass가 유효하면 플레이어 컨트롤러 찾기
	if (PlayerControllerClass)
	{
		for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
		{
			APlayerController* PlayerController = Iterator->Get();
			if (PlayerController && PlayerController->IsA(PlayerControllerClass))
			{
				TargetPlayerController = PlayerController;
				break;
			}
		}

		if (!TargetPlayerController)
		{
			UE_LOG(LogBehaviorTree, Warning, TEXT("%s: Could not find PlayerController of class %s"), *GetName(), *PlayerControllerClass->GetName());
			return EBTNodeResult::Failed;
		}
	}
	else
	{
		// 특정 PlayerController 클래스가 지정되지 않았다면 첫 번째 플레이어 컨트롤러를 찾음
		APlayerController* FirstPlayerController = GetWorld()->GetFirstPlayerController();
		if (FirstPlayerController)
		{
			TargetPlayerController = FirstPlayerController;
		}
		else
		{
			UE_LOG(LogBehaviorTree, Warning, TEXT("%s: No PlayerController found"), *GetName());
			return EBTNodeResult::Failed;
		}
	}


	FVector distance = AIController->GetPawn()->GetActorLocation() - TargetPlayerController->GetPawn()->GetActorLocation() + FVector(0, 0, 400);
	if (distance.Length() < 700) {
		return EBTNodeResult::Succeeded;
	}
	else
	{
		return EBTNodeResult::InProgress;
	}

	 // 지속적인 추적을 위해 InProgress 반환
}

void UBTT_FlyTowardPlayer::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController || !TargetPlayerController)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	APawn* AIPawn = AIController->GetPawn();
	APawn* PlayerPawn = TargetPlayerController->GetPawn();

	if (AIPawn && PlayerPawn)
	{
		// 현재 위치에서 플레이어 위치로 향하는 벡터 계산
		FVector Direction = (PlayerPawn->GetActorLocation() - AIPawn->GetActorLocation() + FVector(0, 0, 400)).GetSafeNormal();

		// 이번 프레임에 이동할 거리 계산
		FVector MovementThisFrame = Direction * FlySpeed * DeltaSeconds;

		// 새로운 위치 계산
		FVector NewLocation = AIPawn->GetActorLocation() + MovementThisFrame;

		// AI Pawn의 위치를 새로운 위치로 설정 (회전은 필요에 따라 추가)
		AIPawn->SetActorLocation(NewLocation);
	}
	else
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}
}

