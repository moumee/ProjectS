// Fill out your copyright notice in the Description page of Project Settings.


#include "Utilities/EnemyTraverseLink.h"

#include "Characters/Enemies/SuraCharacterEnemyBase.h"
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
	
	if (ASuraCharacterEnemyBase* Enemy = Cast<ASuraCharacterEnemyBase>(Agent))
	{
		UE_LOG(LogTemp, Error, TEXT("Traverse LINK REACHED"));
		//PathPoints.Add(Destination);
		WorldPathPoints.Empty();
		for (const FVector& LocalPoint : PathPoints)
		{
			// 3. 각 로컬 좌표를 월드 좌표로 변환합니다.
			const FVector WorldPoint = GetActorTransform().TransformPosition(LocalPoint);
               
			// 4. 변환된 월드 좌표를 새 배열에 추가합니다.
			WorldPathPoints.Add(WorldPoint);
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
