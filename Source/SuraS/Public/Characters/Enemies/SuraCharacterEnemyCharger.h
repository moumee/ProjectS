// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SuraCharacterEnemyBase.h"
#include "SuraCharacterEnemyCharger.generated.h"

class UNiagaraSystem;
class UBoxComponent;

UCLASS()
class SURAS_API ASuraCharacterEnemyCharger : public ASuraCharacterEnemyBase
{
	GENERATED_BODY()

public:
	ASuraCharacterEnemyCharger();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* ChargeReadyAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	UAnimMontage* StunAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	TArray<UAnimMontage*> RoarAnimations;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "OverlapEnemies")
	UBoxComponent* OverlapBox;

	UPROPERTY(EditAnywhere, Category = "NiagaraEffects")
	TSoftObjectPtr<UNiagaraSystem> DashEffect;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void Attack(ASuraPawnPlayer* Player) override;

	void ActivateDashEffect() const;

	UAnimMontage* GetChargeReadyAnimation() const { return ChargeReadyAnimation; };
	UAnimMontage* GetStunAnimation() const { return StunAnimation; };
	UAnimMontage* ChooseRandomRoarMontage();
};
