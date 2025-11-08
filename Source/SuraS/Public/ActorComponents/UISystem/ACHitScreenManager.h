// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ACHitScreenManager.generated.h"


class UNiagaraUIComponent;
class UNiagaraSystem;
class UHitScreenWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SURAS_API UACHitScreenManager : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category= "Hit Screen Widget", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> HighHPEffectWidgetClass; // wbp_damage_1

	UPROPERTY(EditAnywhere, Category= "Hit Screen Widget", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> LowHPEffectWidgetClass; // wbp_damage_2

	UPROPERTY(EditAnywhere, Category = "Hit Screen Widget", meta = (AllowPrivateAccess = "true"))
	float LowHPThreshold = 0.3f;
	
	UPROPERTY(EditAnywhere, Category = "Hit Screen|High HP Niagara Assets")
	TObjectPtr<UNiagaraSystem> NiagaraAsset_99_80;

	UPROPERTY(EditAnywhere, Category = "Hit Screen|High HP Niagara Assets")
	TObjectPtr<UNiagaraSystem> NiagaraAsset_79_50;

	UPROPERTY(EditAnywhere, Category = "Hit Screen|High HP Niagara Assets")
	TObjectPtr<UNiagaraSystem> NiagaraAsset_49_30;

	// 현재 화면에 떠 있는 위젯과 그 안의 나이아가라 컴포넌트 참조
	UPROPERTY()
	TObjectPtr<UHitScreenWidget> CurrentHitWidget;

	UPROPERTY()
	TObjectPtr<UNiagaraUIComponent> CurrentNiagaraComponent;
	

	void UpdateHitScreen(float NewHealth, float OldHealth, float MaxHealth);

	// 애니메이션을 직접 제어하는 함수들
	void TriggerHitFlash();
	void StartBreathingEffect();

	// HitFlash 애니메이션이 끝났을 때 호출될 콜백 함수
	UFUNCTION()
	void OnHitFlashTimerFinished();

	UPROPERTY(EditAnywhere, Category = "Hit Screen|Animation")
	float HitFlashAnimationDuration = 0.2f;

	FTimerHandle HitFlashTimerHandle;
public:	
	// Sets default values for this component's properties
	UACHitScreenManager();
	TObjectPtr<UHitScreenWidget> GetCurrentHitWidget() {return CurrentHitWidget;}

protected:	
	// Called when the game starts
	virtual void BeginPlay() override;

	// 플레이어의 체력 변경 델리게이트에 바인딩할 함수
	UFUNCTION()
	void OnOwnerHealthChanged(float NewHealth, float OldHealth, float MaxHealth, AActor* DamageCauser);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
