// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/DamageComponent/ACDamageSystem.h"
#include "Structures/DamageData.h"

void UACDamageSystem::StartRegeneration()
{
	// 주기적으로 StartHealing을 호출하는 반복 타이머 설정
	GetWorld()->GetTimerManager().SetTimer(
		RegenTimerHandle,
		this,
		&UACDamageSystem::StartHealing,
		TimeUntilRegenStart,
		true
	);
}

void UACDamageSystem::StartHealing()
{
	Heal(RegenHealAmount);
}

// Sets default values for this component's properties
UACDamageSystem::UACDamageSystem()
{
	TimeUntilRegenStart = 5.0f;
	RegenHealAmount = 5.0f;
}

void UACDamageSystem::SetHealth(float val)
{
	Health = val;
}

void UACDamageSystem::SetMaxHealth(float val)
{
	MaxHealth = val;
}

void UACDamageSystem::SetIsInterruptible(bool val)
{
	bIsInterruptible = val;
}

void UACDamageSystem::SetIsInvincible(bool val)
{
	bIsInvincible = val;
}

void UACDamageSystem::SetIsDead(bool val)
{
	bIsDead = val;
}

void UACDamageSystem::Heal(float HealAmount)
{
	if (HealAmount <= 0.0f || IsDead() || Health >= MaxHealth)
	{
		// 체력이 가득 차면 재생 타이머 중지
		if(Health >= MaxHealth)
		{
			GetWorld()->GetTimerManager().ClearTimer(RegenTimerHandle);
		}
		return;
	}
    
	const float OldHealth = Health;
	Health = FMath::Clamp(Health + HealAmount, 0.0f, MaxHealth);
    
	// 체력이 변경되었음을 외부에 알림
	OnHealthChanged.Broadcast(Health, OldHealth, MaxHealth, nullptr);
}

float UACDamageSystem::GetHealthRatio() const
{
	return (MaxHealth > 0.0f) ? Health / MaxHealth : 0.0f;
}

bool UACDamageSystem::IsDead() const
{
	return bIsDead; 
}

bool UACDamageSystem::TakeDamage(const FDamageData& DamageData, AActor* DamageCauser)
{
	// if (!bIsDead && (!bIsInvincible || DamageData.bCanForceDamage))
	// {
	// 	Health -= DamageData.DamageAmount;
	//
	// 	if (Health <= 0.f)
	// 	{
	// 		bIsDead = true;
	// 		OnDeath.Broadcast(); // Call death event
	//
	// 		return true;
	// 	}
	//
	// 	OnDamaged.Broadcast(); // Call damaged event
	//
	// 	return true;
	// }
	//
	// return false;

	if (bIsDead || (bIsInvincible && !DamageData.bCanForceDamage) || DamageData.DamageAmount <= 0.0f)
	{
		return false;
	}

	const float OldHealth = Health;


	UE_LOG(LogTemp, Warning, TEXT("ACDamageSystem: 데미지 %f 입음"), DamageData.DamageAmount);
	Health -= DamageData.DamageAmount;
	Health = FMath::Clamp(Health, 0.0f, MaxHealth);

	UE_LOG(LogTemp, Warning, TEXT("ACDamageSystem: 현재 체력 %f / %f"), Health, MaxHealth);

	if (Health <= 0.f)
	{
		bIsDead = true;
		OnDeath.Broadcast(); // 죽음 이벤트
	}

	OnDamaged.Broadcast(); // 피격 이벤트
	
	OnHealthChanged.Broadcast(Health, OldHealth, MaxHealth, DamageCauser);
	
	GetWorld()->GetTimerManager().ClearTimer(RegenTimerHandle); // 기존 타이머 취소

	if (!bIsDead)
	{
		// TimeUntilRegenStart초 후에 StartRegeneration을 1회 호출
		GetWorld()->GetTimerManager().SetTimer(
		   RegenTimerHandle,
		   this,
		   &UACDamageSystem::StartRegeneration,
		   TimeUntilRegenStart,
		   false
		);
	}
    
	return true;
}

