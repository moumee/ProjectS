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

void ASuraCharacterEnemyCharger::ActivateDashEffect()
{
	if (DashEffect)
	{
		// UE_LOG(LogTemp, Error, TEXT("dash wind should work"));
		
		GetNiagaraComponent()->Deactivate();
		
		NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
			DashEffect.Get(),
			GetMesh(),
			NAME_None,
			FVector(0, -1000, 100),
			FRotator(0, 90, 0),
			EAttachLocation::KeepRelativeOffset,
			true, false, ENCPoolMethod::AutoRelease);

		GetNiagaraComponent()->Activate();
	}
}

void ASuraCharacterEnemyCharger::ActivateCollisionEffect()
{
	if (CollisionEffect)
	{
		// UE_LOG(LogTemp, Error, TEXT("collision fire should work"));
		
		GetNiagaraComponent()->Deactivate();
		
		NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
			CollisionEffect.Get(),
			GetMesh(),
			NAME_None,
			FVector(0, 0, 100),
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
