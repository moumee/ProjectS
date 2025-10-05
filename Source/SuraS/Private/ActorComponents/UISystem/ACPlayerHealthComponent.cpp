// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/UISystem/ACPlayerHealthComponent.h"

// Sets default values for this component's properties
UACPlayerHealthComponent::UACPlayerHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


void UACPlayerHealthComponent::StartHealing()
{
	Heal(RegenHealAmount);
}

void UACPlayerHealthComponent::StartRegeneration()
{
	// 기존 타이머는 비우고, 이제부터 주기적으로 체력을 회복하는 '반복' 타이머를 새로 설정
	GetWorld()->GetTimerManager().ClearTimer(RegenTimerHandle);

	GetWorld()->GetTimerManager().SetTimer(
		RegenTimerHandle,
		this,
		&UACPlayerHealthComponent::StartHealing, // 주기적 회복 함수 호출
		TimeUntilRegenStart,
		true, // true로 설정하여 반복 실행
		0.0f  // 즉시 1회 실행 후, 주기에 맞춰 반복
	);
}

// Called when the game starts
void UACPlayerHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// 게임 시작 시 체력을 가득 채움
	CurrentHealth = MaxHealth;
}


// Called every frame
void UACPlayerHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UACPlayerHealthComponent::TakeDamage(float DamageAmount)
{
	if (DamageAmount <= 0.0f || IsDead())
	{
		return;
	}

	const float OldHealth = CurrentHealth;
	CurrentHealth = FMath::Clamp(CurrentHealth - DamageAmount, 0.0f, MaxHealth);
	
	OnHealthChanged.Broadcast(CurrentHealth, OldHealth, MaxHealth);


	// 데미지를 입었으므로, 이전에 설정된 회복 타이머가 있다면 즉시 취소
	GetWorld()->GetTimerManager().ClearTimer(RegenTimerHandle);

	// 만약 플레이어가 죽지 않았다면, 새로운 회복 타이머를 설정
	if (!IsDead())
	{
		GetWorld()->GetTimerManager().SetTimer(
			RegenTimerHandle,                  
			this,                               
			&UACPlayerHealthComponent::StartRegeneration, 
			TimeUntilRegenStart,                
			false                               
		);
	}
}

void UACPlayerHealthComponent::Heal(float HealAmount)
{
	if (HealAmount <= 0.0f || IsDead() || CurrentHealth >= MaxHealth)
	{
		return;
	}
    
	const float OldHealth = CurrentHealth;
	CurrentHealth = FMath::Clamp(CurrentHealth + HealAmount, 0.0f, MaxHealth);
	
	// 체력이 변경되었음을 외부에 알림
	OnHealthChanged.Broadcast(CurrentHealth, OldHealth, MaxHealth);
}

float UACPlayerHealthComponent::GetHealthRatio() const
{
	return (MaxHealth > 0.0f) ? CurrentHealth / MaxHealth : 0.0f;
}

bool UACPlayerHealthComponent::IsDead() const
{
	return CurrentHealth <= 0.0f;
}

