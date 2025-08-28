// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/Boss/SuraCharacterBossBase.h"

#include "ActorComponents/DamageComponent/ACBossDamageSystem.h"

ASuraCharacterBossBase::ASuraCharacterBossBase()
{
	DamageSystemComponent = CreateDefaultSubobject<UACBossDamageSystem>("DamageSystem");
}

bool ASuraCharacterBossBase::TakeDamage(const FDamageData& DamageData, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Warning, TEXT("Boss Hit Bone Name : %s"), *DamageData.BoneName.ToString());
	
	return GetDamageSystemComp()->TakeDamage(DamageData, DamageCauser);
}

void ASuraCharacterBossBase::Destroyed()
{
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	
	Super::Destroyed();
}
