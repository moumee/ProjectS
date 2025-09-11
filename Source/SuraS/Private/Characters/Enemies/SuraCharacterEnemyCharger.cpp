// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/SuraCharacterEnemyCharger.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Characters/Enemies/AI/Tasks/Attacks/BTT_ChargeAttack.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"


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

void ASuraCharacterEnemyCharger::ActivateDashEffect() const
{
	if (DashEffect)
	{
		GetNiagaraComponent()->Deactivate();
		
		UNiagaraFunctionLibrary::SpawnSystemAttached(
			DashEffect.Get(),
			GetMesh(),
			NAME_None,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::KeepRelativeOffset,
			true, false, ENCPoolMethod::AutoRelease);

		GetNiagaraComponent()->Activate();
	}
}

UAnimMontage* ASuraCharacterEnemyCharger::ChooseRandomRoarMontage()
{
	if (!RoarAnimations.IsEmpty())
		return GetRandomAnimationMontage(RoarAnimations);

	return nullptr;
}
