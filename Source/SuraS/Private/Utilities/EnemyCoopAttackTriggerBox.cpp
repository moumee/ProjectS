// Fill out your copyright notice in the Description page of Project Settings.


#include "Utilities/EnemyCoopAttackTriggerBox.h"

#include "BrainComponent.h"
#include "Characters/SuraCharacterBase.h"
#include "Characters/Enemies/SuraCharacterEnemyBase.h"
#include "Characters/Enemies/AI/EnemyBaseAIController.h"
#include "Components/BoxComponent.h"


// Sets default values
AEnemyCoopAttackTriggerBox::AEnemyCoopAttackTriggerBox()
{
	EnemiesTriggerBox = CreateDefaultSubobject<UBoxComponent>(FName("Enemies Trigger Box"));
	RootComponent = EnemiesTriggerBox;
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

// Called when the game starts or when spawned
void AEnemyCoopAttackTriggerBox::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerTriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCoopAttackTriggerBox::OnPlayerOverlapBegin);
	PlayerTriggerBox->OnComponentEndOverlap.AddDynamic(this, &AEnemyCoopAttackTriggerBox::OnPlayerOverlapEnd);
}

void AEnemyCoopAttackTriggerBox::OnEnemiesOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ASuraCharacterEnemyBase* Enemy = Cast<ASuraCharacterEnemyBase>(OtherActor))
	{
		Enemy->GetAIController()->EndPursueState();
		Enemy->GetAIController()->GetBrainComponent()->StopLogic("CoopAttack");
		EnemiesForCoopAttack.Add(Enemy);

		if (EnemiesForCoopAttack.Num() >= 2)
		{
			// UE_LOG(LogTemp, Error, TEXT("Enough for coop attack"));

			EnemiesForCoopAttack[0]->GetAIController()->GetBrainComponent()->RestartLogic();
			EnemiesForCoopAttack[1]->GetAIController()->GetBrainComponent()->RestartLogic();
			
			EnemiesForCoopAttack[0]->GetAIController()->SetStateToCoopAttack(EnemiesForCoopAttack[1], true);
			EnemiesForCoopAttack[1]->GetAIController()->SetStateToCoopAttack(EnemiesForCoopAttack[0], false);

			EnemiesForCoopAttack.Empty();

			// UE_LOG(LogTemp, Log, TEXT("EnemiesForCoopAttack length %d"), EnemiesForCoopAttack.Num());
		}

		// 1. Find another enemy in a radius
		// 1-1. if the found enemy is pursuing, stop that enemy on contact with the thrower enemy
		// 2. use motion warping when close enough to play throwing animation
		// 3. thrown enemy hit with player, and become dead as soon as making contact with the ground.
		// 4. if no enemy, jump thyself
	}
}

void AEnemyCoopAttackTriggerBox::OnPlayerOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<ASuraPawnPlayer>(OtherActor))
	{
		EnemiesTriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCoopAttackTriggerBox::OnEnemiesOverlapBegin);
	}
}

void AEnemyCoopAttackTriggerBox::OnPlayerOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (ASuraPawnPlayer* Player = Cast<ASuraPawnPlayer>(OtherActor))
	{
		EnemiesTriggerBox->OnComponentBeginOverlap.RemoveAll(this);

		if (!EnemiesForCoopAttack.IsEmpty())
		{
			for (auto Enemy : EnemiesForCoopAttack)
			{
				Enemy->GetAIController()->GetBrainComponent()->RestartLogic();
				Enemy->GetAIController()->SetStateToChaseOrPursue(Player);
			}

			EnemiesForCoopAttack.Empty();
		}
	}
}
