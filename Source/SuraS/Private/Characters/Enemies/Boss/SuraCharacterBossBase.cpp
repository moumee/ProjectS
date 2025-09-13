// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/Boss/SuraCharacterBossBase.h"

#include "ActorComponents/DamageComponent/ACBossDamageSystem.h"



ASuraCharacterBossBase::ASuraCharacterBossBase()
{
	DamageSystemComponent = CreateDefaultSubobject<UACBossDamageSystem>("DamageSystem");
}

bool ASuraCharacterBossBase::TakeDamage(const FDamageData& DamageData, AActor* DamageCauser)
{
	return GetDamageSystemComp()->TakeDamage(DamageData, DamageCauser);
}

void ASuraCharacterBossBase::Destroyed()
{
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	
	Super::Destroyed();
}

