// Fill out your copyright notice in the Description page of Project Settings.


#include "Utilities/EnemyTriggerBox.h"

#include "Characters/Enemies/SuraCharacterEnemyBase.h"
#include "Characters/Enemies/AI/EnemyBaseAIController.h"
#include "Characters/PawnBAsePlayer/SuraPawnPlayer.h"


// Sets default values
AEnemyTriggerBox::AEnemyTriggerBox()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(FName("TriggerBox"));
	RootComponent = TriggerBox;
	TriggerBox->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	TriggerBox->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void AEnemyTriggerBox::BeginPlay()
{
	Super::BeginPlay();

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AEnemyTriggerBox::OnOverlapBegin);
}

// Called every frame
void AEnemyTriggerBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyTriggerBox::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		if (ASuraPawnPlayer* const Player = Cast<ASuraPawnPlayer>(OtherActor))
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Player Overlapped"));

			for (ASuraCharacterEnemyBase* Enemy : EnemiesToTrigger)
			{
				Enemy->GetAIController()->SetStateToChaseOrPursue(Player);
			}
		}
	}
}

