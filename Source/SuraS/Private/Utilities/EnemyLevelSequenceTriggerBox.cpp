// Fill out your copyright notice in the Description page of Project Settings.


#include "Utilities/EnemyLevelSequenceTriggerBox.h"

#include "LevelSequencePlayer.h"
#include "MovieSceneSequencePlaybackSettings.h"
#include "Characters/PawnBasePlayer/SuraPawnPlayer.h"
#include "Components/BoxComponent.h"


// Sets default values
AEnemyLevelSequenceTriggerBox::AEnemyLevelSequenceTriggerBox()
{
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(FName("Trigger Box"));
	RootComponent = TriggerBox;
	TriggerBox->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	TriggerBox->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore); // Ignore Projectile
	TriggerBox->SetCollisionResponseToChannel(ECC_GameTraceChannel6, ECR_Overlap); // Ignore Enemies Overlap channel
	TriggerBox->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void AEnemyLevelSequenceTriggerBox::BeginPlay()
{
	Super::BeginPlay();

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AEnemyLevelSequenceTriggerBox::OnOverlapBegin);
}

void AEnemyLevelSequenceTriggerBox::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<ASuraPawnPlayer>(OtherActor))
	{
		FMovieSceneSequencePlaybackSettings LSPlaybackSettings;

		if (LevelSequenceAsset)
		{
			ALevelSequenceActor* OutActor;
			LevelSequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), LevelSequenceAsset, LSPlaybackSettings, OutActor);

			if (LevelSequencePlayer)
			{
				LevelSequencePlayer->Play();
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Unable to create level sequence player"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Unable to find level sequence asset"));
		}
	}
}
