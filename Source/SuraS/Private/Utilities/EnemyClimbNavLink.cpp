// Fill out your copyright notice in the Description page of Project Settings.


#include "Utilities/EnemyClimbNavLink.h"
#include "Characters/Enemies/SuraCharacterEnemyBase.h"
#include "Characters/Enemies/AI/EnemyBaseAIController.h"
#include "Components/SphereComponent.h"

// Sets default values
AEnemyClimbNavLink::AEnemyClimbNavLink()
{
	SetSmartLinkEnabled(true);
	bSmartLinkIsRelevant = true;

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	EndSphere = CreateDefaultSubobject<USphereComponent>(FName("EndSphere"));
	EndSphere->SetupAttachment(RootComponent);
	EndSphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	EndSphere->SetSphereRadius(10.f);
	EndSphere->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore); // Ignore Projectile
	EndSphere->SetCollisionResponseToChannel(ECC_GameTraceChannel6, ECR_Overlap);
	EndSphere->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void AEnemyClimbNavLink::BeginPlay()
{
	Super::BeginPlay();

	OnSmartLinkReached.AddDynamic(this, &AEnemyClimbNavLink::OnReceiveSmartLinkReached);
	EndSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemyClimbNavLink::OnOverlapBegin);
}

void  AEnemyClimbNavLink::OnReceiveSmartLinkReached(AActor* Agent, const FVector& Destination)
{
	// let the enemy jump off the ledge if on the higher ground
	if (Destination.Z < Agent->GetActorLocation().Z)
		return;
	
	/*if (ASuraCharacterEnemyBase* const Enemy = Cast<ASuraCharacterEnemyBase>(Agent))
	{
		if (bIsOccupied && OccupyingEnemy.Get() != Enemy)
			return;
		
		OccupyingEnemy = Enemy;

		/*UCrowdFollowingComponent* CrowdComp = Cast<UCrowdFollowingComponent>(OccupyingEnemy->GetAIController()->GetPathFollowingComponent());
		if (CrowdComp)
			CrowdComp->PauseMove(FAIRequestID::CurrentRequest);#1#
			//CrowdComp->SetCrowdSimulationState(ECrowdSimulationState::Disabled); // Disable crowd AI
		
		// SetLinkUsable(false);
	}
	else
		return;*/

	// have the enemy climb up the wall
	if (ASuraCharacterEnemyBase* const Enemy = Cast<ASuraCharacterEnemyBase>(Agent))
	{
		Enemy->Climb(Destination);
		
		/*Enemy->GetAIController()->ClearFocus(EAIFocusPriority::Gameplay);
		Enemy->SetActorRotation(FRotator(Enemy->GetActorRotation().Pitch, GetActorRotation().Yaw, Enemy->GetActorRotation().Roll));
		
		if (Enemy->GetAIController()->GetCurrentState() == EEnemyStates::Pursue || Enemy->GetAIController()->GetCurrentState() == EEnemyStates::Attacking)
			Enemy->GetAIController()->EndPursueState();
			
		Enemy->GetAIController()->UpdateCurrentState(EEnemyStates::Climbing);*/
	}
}

void AEnemyClimbNavLink::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, OtherActor->GetName());
	
	if (const ASuraCharacterEnemyBase* Enemy = Cast<ASuraCharacterEnemyBase>(OtherActor))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Enemy Overlapped"));
		UE_LOG(LogTemp, Warning, TEXT("Enemy Overlapped"));

		if (Enemy == OccupyingEnemy)
			SetLinkUsable(true);
	}
}

void AEnemyClimbNavLink::SetLinkUsable(bool bIsUsable)
{
	bIsOccupied = !bIsUsable;
	SetSmartLinkEnabled(bIsUsable);

	if (bIsUsable)
		OccupyingEnemy.Reset();
}