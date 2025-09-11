// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/AI/EnemyAdvancedJumpNavLinkProxy.h"
#include "NavigationSystem.h"
#include "Characters/Enemies/SuraCharacterEnemyBase.h"
#include "Characters/Enemies/AI/EnemyBaseAIController.h"
#include "Characters/Enemies/AI/EnemySequentialJumpComponent.h"

class ASuraCharacterEnemyBase;

AEnemyAdvancedJumpNavLinkProxy::AEnemyAdvancedJumpNavLinkProxy()
{
	SetSmartLinkEnabled(true);
	bSmartLinkIsRelevant = true;
	PointLinks.Empty();
}

void AEnemyAdvancedJumpNavLinkProxy::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	//OnSmartLinkReached.AddDynamic(this, &AEnemyAdvancedJumpNavLinkProxy::OnReceiveSmartLinkReached);
	
	UpdateAndSynchronizeLinks(); // 함수 이름 변경
}

void AEnemyAdvancedJumpNavLinkProxy::BeginPlay()
{
	Super::BeginPlay();

	OnSmartLinkReached.AddDynamic(this, &AEnemyAdvancedJumpNavLinkProxy::OnReceiveSmartLinkReached);
}

#if WITH_EDITOR
void AEnemyAdvancedJumpNavLinkProxy::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	UpdateAndSynchronizeLinks(); // 함수 이름 변경
}
#endif

void AEnemyAdvancedJumpNavLinkProxy::UpdateAndSynchronizeLinks() // 함수 내용 완전 변경
{
	// 기존의 모든 링크를 지웁니다.
	PointLinks.Empty();
	
	// 경로점이 2개 이상일 때만 링크를 생성합니다.
	if (PathPoints.Num() >= 2)
	{
		// 경로점을 순회하며 필요한 링크 쌍을 생성합니다.
		for (int32 i = 0; i < PathPoints.Num() - 1; ++i)
		{
			FNavigationLink NewLink;
			NewLink.Left = PathPoints[i];
			NewLink.Right = PathPoints[i+1];
			// 양방향으로 이동 가능하도록 설정할 수 있습니다.
			NewLink.Direction = ENavLinkDirection::BothWays; 
            
			PointLinks.Add(NewLink);
		}
	}
    
	// 네비게이션 시스템에 변경사항을 알립니다.
	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (NavSys)
	{
		NavSys->UpdateActorInNavOctree(*this);
	}
}

void AEnemyAdvancedJumpNavLinkProxy::OnReceiveSmartLinkReached(AActor* Agent, const FVector& Destination)
{
	UE_LOG(LogTemp, Error, TEXT("Traverse LINK REACHED"));
	if (ASuraCharacterEnemyBase* Enemy = Cast<ASuraCharacterEnemyBase>(Agent))
	{
		//Enemy->FindComponentByClass<UEnemySequentialJumpComponent>()->PathPoints = PathPoints;
		Enemy->GetAIController()->UpdateCurrentState(EEnemyStates::Traverse);
		
	}
	
	
	// UE_LOG(LogTemp, Error, TEXT("JUMP LINK REACHED"));
	// if (Destination.Z < Agent->GetActorLocation().Z)
	// 	return;
	//
	// if (ASuraCharacterEnemyBase* Enemy = Cast<ASuraCharacterEnemyBase>(Agent))
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("JUMP LINK REACHED"));
	// 	
	// 	//Enemy->JumpWall(Destination);
	// }
}