// Fill out your copyright notice in the Description page of Project Settings.


#include "Utilities/EnemyEndClimbingTrigger.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemies/SuraCharacterEnemyBase.h"
#include "Characters/Enemies/AI/EnemyBaseAIController.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Utilities/EnemyClimbNavLink.h"


// Sets default values
AEnemyEndClimbingTrigger::AEnemyEndClimbingTrigger()
{
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(FName("TriggerBox"));
	RootComponent = TriggerBox;
	TriggerBox->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	TriggerBox->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore); // Ignore Projectile
	TriggerBox->SetCollisionResponseToChannel(ECC_GameTraceChannel6, ECR_Overlap); // Ignore Enemies Overlap channel
	TriggerBox->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void AEnemyEndClimbingTrigger::BeginPlay()
{
	Super::BeginPlay();

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AEnemyEndClimbingTrigger::OnOverlapBegin);
}

void AEnemyEndClimbingTrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ASuraCharacterEnemyBase* const Enemy = Cast<ASuraCharacterEnemyBase>(OtherActor))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Enemy Climb End Overlapped"));

		Enemy->SetActorRotation(Enemy->GetAIController()->GetBlackboardComponent()->GetValueAsRotator("TargetRotation"));
		Enemy->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		Enemy->GetCharacterMovement()->StopMovementImmediately();
		Enemy->GetAIController()->SetStateToChaseOrPursue(Enemy->GetAIController()->GetAttackTarget());

		if (!ClimbNavLinks.IsEmpty())
		{
			for (AEnemyClimbNavLink* ClimbNavLink : ClimbNavLinks)
			{
				if (ClimbNavLink->GetCachedEnemy() == Enemy)
				{
					ClimbNavLink->SetLinkUsable(true);
				}
			}
		}
	}
}

