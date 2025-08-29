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
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(FName("TriggerBox"));
	RootComponent = TriggerBox;
	TriggerBox->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	TriggerBox->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore); // Ignore Projectile
	TriggerBox->SetCollisionResponseToChannel(ECC_GameTraceChannel6, ECR_Overlap); // Ignore Enemies Overlap channel
	TriggerBox->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void AEnemyCoopAttackTriggerBox::BeginPlay()
{
	Super::BeginPlay();

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCoopAttackTriggerBox::OnOverlapBegin);
}

void AEnemyCoopAttackTriggerBox::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ASuraCharacterEnemyBase* Enemy = Cast<ASuraCharacterEnemyBase>(OtherActor))
	{
		Enemy->GetAIController()->EndPursueState();
		Enemy->GetAIController()->GetBrainComponent()->StopLogic("CoopAttack");
		EnemiesForCoopAttack.Add(Enemy);

		if (EnemiesForCoopAttack.Num() >= 2)
		{
			UE_LOG(LogTemp, Error, TEXT("Enough for coop attack"));

			for (auto EnemyForCoopAttack : EnemiesForCoopAttack)
			{
				EnemyForCoopAttack->GetAIController()->Set
			}
		}

		// 1. Find another enemy in a radius
		// 1-1. if the found enemy is pursuing, stop that enemy on contact with the thrower enemy
		// 2. use motion warping when close enough to play throwing animation
		// 3. thrown enemy hit with player, and become dead as soon as making contact with the ground.
		// 4. if no enemy, jump thyself
	}
}

