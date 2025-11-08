// Fill out your copyright notice in the Description page of Project Settings.


#include "Utilities/EnemyCoopAttackTriggerBox.h"

#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/Enemies/SuraCharacterEnemyBase.h"
#include "Characters/Enemies/AI/EnemyBaseAIController.h"
#include "Components/BoxComponent.h"

// Sets default values
AEnemyCoopAttackTriggerBox::AEnemyCoopAttackTriggerBox()
{
	TriggerRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = TriggerRootComponent;
	
	EnemiesTriggerBox = CreateDefaultSubobject<UBoxComponent>(FName("Enemies Trigger Box"));
	EnemiesTriggerBox->SetupAttachment(RootComponent);
	EnemiesTriggerBox->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	EnemiesTriggerBox->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore); // Ignore Projectile
	EnemiesTriggerBox->SetCollisionResponseToChannel(ECC_GameTraceChannel6, ECR_Overlap); // Ignore Enemies Overlap channel
	EnemiesTriggerBox->SetGenerateOverlapEvents(true);

	PlayerTriggerBox = CreateDefaultSubobject<UBoxComponent>(FName("Player Trigger Box"));
	PlayerTriggerBox->SetupAttachment(RootComponent);
	PlayerTriggerBox->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	PlayerTriggerBox->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore); // Ignore Projectile
	PlayerTriggerBox->SetCollisionResponseToChannel(ECC_GameTraceChannel6, ECR_Overlap); // Ignore Enemies Overlap channel
	PlayerTriggerBox->SetGenerateOverlapEvents(true);
}

void AEnemyCoopAttackTriggerBox::ActivateCoopAttack()
{
	TArray<ASuraCharacterEnemyBase*> AvailableEnemies;
	for (auto Enemy : EnemiesForCoopAttack)
	{
		if (Enemy.Get() && Enemy->GetAIController() && Enemy->GetAIController()->GetCurrentState() != EEnemyStates::CoopAttacking)
		{
			AvailableEnemies.AddUnique(Enemy.Get());
		}
	}

	for (auto Enemy : AvailableEnemies)
	{
		Enemy->GetAIController()->EndPursueState();
		Enemy->GetAIController()->GetBrainComponent()->StopLogic("CoopAttack");
	}

	// Coop Attack Start
	if (AvailableEnemies.Num() >= 2)
	{
		while (AvailableEnemies.Num() >= 2)
		{
			// UE_LOG(LogTemp, Error, TEXT("Enough for coop attack"));
			ASuraCharacterEnemyBase* EnemyA = AvailableEnemies[0];
			ASuraCharacterEnemyBase* EnemyB = AvailableEnemies[1];

			EnemyA->GetAIController()->GetBrainComponent()->RestartLogic();
			EnemyB->GetAIController()->GetBrainComponent()->RestartLogic();

			EnemyA->GetAIController()->GetBlackboardComponent()->SetValueAsRotator(
				"TargetRotation",
				FRotator(0, EnemiesForCoopAttack[0]->GetActorRotation().Yaw, 0)
				);
		
			EnemyB->GetAIController()->GetBlackboardComponent()->SetValueAsRotator(
				"TargetRotation",
				FRotator(0, EnemiesForCoopAttack[1]->GetActorRotation().Yaw, 0)
				); // to straighten the climbing rotation

			if (EnemyA != EnemyB)
			{
				EnemyA->GetAIController()->SetStateToCoopAttack(EnemyB, true);
				EnemyB->GetAIController()->SetStateToCoopAttack(EnemyA, false);
			}

			// possibly reactivate the remaining enemies?

			AvailableEnemies.RemoveAt(1);
			AvailableEnemies.RemoveAt(0);

			// UE_LOG(LogTemp, Log, TEXT("EnemiesForCoopAttack length %d"), EnemiesForCoopAttack.Num());
		}

		if (AvailableEnemies.Num() > 0)
		{
			ASuraCharacterEnemyBase* Loner = AvailableEnemies[0];
			Loner->GetAIController()->GetBrainComponent()->RestartLogic();

			Loner->GetAIController()->SetStateToChaseOrPursue(Loner);
		}
	}
	else
	{
		for (auto Enemy : AvailableEnemies)
		{
			Enemy->GetAIController()->GetBrainComponent()->RestartLogic();
			Enemy->GetAIController()->SetStateToChaseOrPursue(Enemy);
		}
	}

	// 1. Find another enemy in a radius
	// 1-1. if the found enemy is pursuing, stop that enemy on contact with the thrower enemy
	// 2. use motion warping when close enough to play throwing animation
	// 3. thrown enemy hit with player, and become dead as soon as making contact with the ground.
	// 4. if no enemy, jump thyself
}

// Called when the game starts or when spawned
void AEnemyCoopAttackTriggerBox::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerTriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCoopAttackTriggerBox::OnPlayerOverlapBegin);
	PlayerTriggerBox->OnComponentEndOverlap.AddDynamic(this, &AEnemyCoopAttackTriggerBox::OnPlayerOverlapEnd);
	EnemiesTriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCoopAttackTriggerBox::OnEnemyOverlapBegin);
	EnemiesTriggerBox->OnComponentEndOverlap.AddDynamic(this, &AEnemyCoopAttackTriggerBox::OnEnemyOverlapEnd);
}

void AEnemyCoopAttackTriggerBox::OnEnemyOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ASuraCharacterEnemyBase* Enemy = Cast<ASuraCharacterEnemyBase>(OtherActor))
	{
		EnemiesForCoopAttack.AddUnique(Enemy);

		if (bIsPlayerInsideTrigger)
			ActivateCoopAttack(); 
	}
}

void AEnemyCoopAttackTriggerBox::OnEnemyOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ASuraCharacterEnemyBase* Enemy = Cast<ASuraCharacterEnemyBase>(OtherActor))
	{
		EnemiesForCoopAttack.Remove(Enemy);
		
		Enemy->GetAIController()->GetBrainComponent()->RestartLogic();
		Enemy->GetAIController()->SetStateToChaseOrPursue(Enemy);
	}
}

void AEnemyCoopAttackTriggerBox::OnPlayerOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<ASuraPawnPlayer>(OtherActor))
	{
		TArray<AActor*> OutActors;
		EnemiesTriggerBox->GetOverlappingActors(OutActors, ASuraCharacterEnemyBase::StaticClass());

		EnemiesForCoopAttack.Empty();

		for (AActor* Actor : OutActors)
		{
			if (ASuraCharacterEnemyBase* EnemyActor = Cast<ASuraCharacterEnemyBase>(Actor))
				EnemiesForCoopAttack.AddUnique(EnemyActor);
		}
		
		bIsPlayerInsideTrigger = true;
		ActivateCoopAttack();
	}
}

void AEnemyCoopAttackTriggerBox::OnPlayerOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ASuraPawnPlayer* Player = Cast<ASuraPawnPlayer>(OtherActor))
	{
		bIsPlayerInsideTrigger = false;
		
		if (!EnemiesForCoopAttack.IsEmpty())
		{
			for (auto Enemy : EnemiesForCoopAttack)
			{
				if (Enemy->GetAIController()->GetCurrentState() != EEnemyStates::CoopAttacking)
				{
					Enemy->GetAIController()->GetBrainComponent()->RestartLogic();
					Enemy->GetAIController()->SetStateToChaseOrPursue(Player);
				}
			}

			EnemiesForCoopAttack.Empty();
		}
	}
}

