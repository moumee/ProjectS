// Fill out your copyright notice in the Description page of Project Settings.


#include "Utilities/EnemyTraverseLink.h"

#include "Characters/Enemies/SuraCharacterEnemyBase.h"
#include "Characters/Enemies/SuraCharacterEnemyMelee.h"
#include "Characters/Enemies/SuraCharacterEnemyRifle.h"
#include "Characters/Enemies/AI/EnemyBaseAIController.h"
#include "Characters/Enemies/AI/EnemySequentialJumpComponent.h"

void AEnemyTraverseLink::OnCachedEnemyDeath()
{
	SetSmartLinkEnabled(true);
	bSmartLinkIsRelevant = true;
}

void AEnemyTraverseLink::BeginPlay()
{
	Super::BeginPlay();
	OnSmartLinkReached.AddDynamic(this, &AEnemyTraverseLink::OnReceiveSmartLinkReached);
}

void AEnemyTraverseLink::OnReceiveSmartLinkReached(AActor* Agent, const FVector& Destination)
{
	
	if (ASuraCharacterEnemyMelee* Enemy = Cast<ASuraCharacterEnemyMelee>(Agent))
	{
		//if (Cast<ASuraCharacterEnemyRifle>(Agent))return;
		UE_LOG(LogTemp, Error, TEXT("Traverse LINK REACHED"));

		if (Enemy->GetAIController()->GetCurrentState() == EEnemyStates::Pursue || Enemy->GetAIController()->GetCurrentState() == EEnemyStates::Attacking)
			Enemy->GetAIController()->EndPursueState();
		//PathPoints.Add(Destination);
		WorldPathPoints.Empty();
		if (FVector::DistSquared(Destination, GetTransform().TransformPosition(PointLinks[0].Right))
			> FVector::DistSquared(Destination, GetTransform().TransformPosition(PointLinks[0].Left)))
		{
			for (const FVector& LocalPoint : PathPoints)
			{
				//각 로컬 좌표를 월드 좌표로 변환
				const FVector WorldPoint = GetActorTransform().TransformPosition(LocalPoint);
				WorldPathPoints.Add(WorldPoint);
			}
		}
		else
		{
			//거꾸로 불러오기
			for (auto It = PathPoints.rbegin(); It != PathPoints.rend(); ++It)
			{
				//각 로컬 좌표를 월드 좌표로 변환
				const FVector WorldPoint = GetActorTransform().TransformPosition(*It);
				WorldPathPoints.Add(WorldPoint);
			}
		}
		
		WorldPathPoints.Add(Destination);
		Enemy->FindComponentByClass<UEnemySequentialJumpComponent>()->SetPathPoints(WorldPathPoints);
		Enemy->GetAIController()->UpdateCurrentState(EEnemyStates::Traverse);
		
	}
}

void AEnemyTraverseLink::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	
}

AEnemyTraverseLink::AEnemyTraverseLink()
{
	SetSmartLinkEnabled(true);
	bSmartLinkIsRelevant = true;
}

void AEnemyTraverseLink::SetLinkUsable(bool bIsUsable)
{
	SetSmartLinkEnabled(bIsUsable);

	if (!bIsUsable)
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Enemy Climb disabled"));

	if (bIsUsable)
		CachedEnemy = nullptr;
}
