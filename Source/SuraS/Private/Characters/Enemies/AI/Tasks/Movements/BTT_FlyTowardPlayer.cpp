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
	// TickTask Ȱ��ȭ
	bNotifyTick = true;
	TargetPlayerController = nullptr;
	FlySpeed = 2.0f; // �⺻ ���� �ӵ� ����
}

EBTNodeResult::Type UBTT_FlyTowardPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController)
	{
		return EBTNodeResult::Failed;
	}

	// PlayerControllerClass�� ��ȿ�ϸ� �÷��̾� ��Ʈ�ѷ� ã��
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
		// Ư�� PlayerController Ŭ������ �������� �ʾҴٸ� ù ��° �÷��̾� ��Ʈ�ѷ��� ã��
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

	 // �������� ������ ���� InProgress ��ȯ
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
		// ���� ��ġ���� �÷��̾� ��ġ�� ���ϴ� ���� ���
		FVector Direction = (PlayerPawn->GetActorLocation() - AIPawn->GetActorLocation() + FVector(0, 0, 400)).GetSafeNormal();

		// �̹� �����ӿ� �̵��� �Ÿ� ���
		FVector MovementThisFrame = Direction * FlySpeed * DeltaSeconds;

		// ���ο� ��ġ ���
		FVector NewLocation = AIPawn->GetActorLocation() + MovementThisFrame;

		// AI Pawn�� ��ġ�� ���ο� ��ġ�� ���� (ȸ���� �ʿ信 ���� �߰�)
		AIPawn->SetActorLocation(NewLocation);
	}
	else
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}
}

