// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/SuraCharacterEnemyCharger.h"

#include "Components/BoxComponent.h"


// Sets default values
ASuraCharacterEnemyCharger::ASuraCharacterEnemyCharger()
{
	EnemyType = "Charger";

	OverlapBox = CreateDefaultSubobject<UBoxComponent>(FName("OverlapBox"));
	OverlapBox->SetupAttachment(RootComponent);
	OverlapBox->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	OverlapBox->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore); // Ignore Projectile
	OverlapBox->SetCollisionResponseToChannel(ECC_GameTraceChannel6, ECR_Overlap); // Ignore Enemies Overlap channel
	OverlapBox->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void ASuraCharacterEnemyCharger::BeginPlay()
{
	Super::BeginPlay();
}

void ASuraCharacterEnemyCharger::Attack(ASuraPawnPlayer* Player)
{
	Super::Attack(Player);
}

UAnimMontage* ASuraCharacterEnemyCharger::ChooseRandomRoarMontage()
{
	if (!RoarAnimations.IsEmpty())
		return GetRandomAnimationMontage(RoarAnimations);

	return nullptr;
}
