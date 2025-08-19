// Fill out your copyright notice in the Description page of Project Settings.


#include "Utilities/EnemyJumpNavLink.h"

#include "Characters/Enemies/SuraCharacterEnemyBase.h"


// Sets default values
AEnemyJumpNavLink::AEnemyJumpNavLink()
{
	SetSmartLinkEnabled(true);
	bSmartLinkIsRelevant = true;
}

// Called when the game starts or when spawned
void AEnemyJumpNavLink::BeginPlay()
{
	Super::BeginPlay();

	OnSmartLinkReached.AddDynamic(this, &AEnemyJumpNavLink::OnReceiveSmartLinkReached);
}

void AEnemyJumpNavLink::OnReceiveSmartLinkReached(AActor* Agent, const FVector& Destination)
{
	if (Destination.Z < Agent->GetActorLocation().Z)
		return;

	if (ASuraCharacterEnemyBase* Enemy = Cast<ASuraCharacterEnemyBase>(Agent))
	{
		UE_LOG(LogTemp, Warning, TEXT("JUMP LINK REACHED"));
		
		Enemy->JumpWall(Destination);
	}
}