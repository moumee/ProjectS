// Fill out your copyright notice in the Description page of Project Settings.


#include "Utilities/EnemyClimbNavLink.h"
#include "NavModifierComponent.h"
#include "Characters/Enemies/SuraCharacterEnemyBase.h"
#include "NavAreas/NavArea_Default.h"

// Sets default values
AEnemyClimbNavLink::AEnemyClimbNavLink()
{
	NavModifierComponent = CreateDefaultSubobject<UNavModifierComponent>(TEXT("NavModifierComponent"));
	AddOwnedComponent(NavModifierComponent);

	NavModifierComponent->SetAreaClass(UNavArea_Default::StaticClass());
	NavModifierComponent->FailsafeExtent = FVector(30.f, 30.f, 150.f);
	
	SetSmartLinkEnabled(true);
	bSmartLinkIsRelevant = true;
}

// Called when the game starts or when spawned
void AEnemyClimbNavLink::BeginPlay()
{
	Super::BeginPlay();

	OnSmartLinkReached.AddDynamic(this, &AEnemyClimbNavLink::OnReceiveSmartLinkReached);
}

void  AEnemyClimbNavLink::OnReceiveSmartLinkReached(AActor* Agent, const FVector& Destination)
{
	// let the enemy jump off the ledge if on the higher ground
	if (Destination.Z < Agent->GetActorLocation().Z)
		return;

	// have the enemy climb up the wall
	if (ASuraCharacterEnemyBase* const Enemy = Cast<ASuraCharacterEnemyBase>(Agent))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%s"), *Destination.ToString()));
		
		Enemy->Climb(Destination);
	}
}
