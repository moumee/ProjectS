// Fill out your copyright notice in the Description page of Project Settings.


#include "Utilities/ArenaLock.h"

#include "ActorComponents/DamageComponent/ACEnemyDamageSystem.h"
#include "Characters/Enemies/SuraCharacterEnemyBase.h"

AArenaLock::AArenaLock()
{
	ArenaMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("ArenaMesh"));
	RootComponent = ArenaMesh;
	ArenaMesh->Mobility = EComponentMobility::Static;
	ArenaMesh->SetSimulatePhysics(false);
	ArenaMesh->SetCollisionProfileName(TEXT("BlockAll"));

	DissolveTimeline = CreateDefaultSubobject<UTimelineComponent>("DissolveColorTimeline");
}

void AArenaLock::BeginPlay()
{
	Super::BeginPlay();

	if (!ArenaEnemies.IsEmpty())
	{
		for (ASuraCharacterEnemyBase* Enemy : ArenaEnemies)
		{
			Enemy->GetDamageSystemComp()->OnDeath.AddWeakLambda(this, [this, Enemy]()
			{
				this->OnEnemyDeath(Enemy);
			});
		}
	}

	if (DissolveColorCurve)
	{
		OnDissolveColorTimelineFloat.BindUFunction(this, FName("UpdateDissolveColor"));
		DissolveTimeline->AddInterpFloat(DissolveColorCurve, OnDissolveColorTimelineFloat);
	}
}

void AArenaLock::UpdateDissolveColor(float Alpha)
{
	ArenaMesh->SetScalarParameterValueOnMaterials("DissolveAlpha", Alpha);
}

void AArenaLock::OnEnemyDeath(ASuraCharacterEnemyBase* Enemy)
{
	if (Enemy)
	{
		ArenaEnemies.Remove(Enemy);

		if (ArenaEnemies.IsEmpty())
		{
			DissolveTimeline->PlayFromStart();

			FTimerHandle DeathHandle;
			
			GetWorldTimerManager().SetTimer(
				DeathHandle,
				FTimerDelegate::CreateWeakLambda(this, [this]()
				{
					Destroy();
				}),
			2,
		false);
		}
	}
}
