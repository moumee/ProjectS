// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SuraCharacterEnemyBase.h"
#include "SuraCharacterEnemyCharger.generated.h"

UCLASS()
class SURAS_API ASuraCharacterEnemyCharger : public ASuraCharacterEnemyBase
{
	GENERATED_BODY()

public:
	ASuraCharacterEnemyCharger();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* ChargeReadyAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* StunAnimation;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void Attack(ASuraPawnPlayer* Player) override;

	UAnimMontage* GetChargeReadyAnimation() const { return ChargeReadyAnimation; };
	UAnimMontage* GetStunAnimation() const { return StunAnimation; };
};
