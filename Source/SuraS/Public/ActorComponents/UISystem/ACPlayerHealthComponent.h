// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ACPlayerHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnHealthChangedSignature, float, NewHealth, float, OldHealth, float, MaxHealth);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SURAS_API UACPlayerHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UACPlayerHealthComponent();
	

private:
	UPROPERTY(BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = "true"))
	float CurrentHealth = 0.f;

	// 회복 타이머를 관리하기 위한 핸들
	FTimerHandle RegenTimerHandle;

	// 타이머가 끝나면 실제로 회복을 시작시키는 함수
	void StartHealing();

	// 회복 대기 시간이 끝나면 호출될 함수
	void StartRegeneration();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health|Regeneration")
	float TimeUntilRegenStart = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health|Regeneration")
	float RegenHealAmount = 10.0f;
	

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnHealthChangedSignature OnHealthChanged;
	
	UFUNCTION(BlueprintCallable, Category = "Health")
	void TakeDamage(float DamageAmount);

	UFUNCTION(BlueprintCallable, Category = "Health")
	void Heal(float HealAmount);
    
	UFUNCTION(BlueprintPure, Category = "Health")
	float GetHealthRatio() const;

	UFUNCTION(BlueprintPure, Category = "Health")
	bool IsDead() const;

	UFUNCTION(BlueprintPure, Category = "Health")
	float GetCurrentHealth() const {return CurrentHealth;}

	UFUNCTION(BlueprintPure, Category = "Health")
	float GetMaxHealth() const {return MaxHealth;}
	
};
