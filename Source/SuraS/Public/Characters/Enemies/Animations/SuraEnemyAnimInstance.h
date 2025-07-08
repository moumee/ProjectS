// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SuraEnemyAnimInstance.generated.h"

class AEnemyBaseAIController;
enum class EEnemyStates : uint8;
class ASuraCharacterEnemyBase;
/**
 * 
 */
UCLASS()
class SURAS_API USuraEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Enemy")
	ASuraCharacterEnemyBase* Enemy;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FVector Velocity;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float Direction;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bIsPursueOrAttacking = false;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	EEnemyStates EnemyState;

	UPROPERTY()
	AEnemyBaseAIController* EnemyBaseAIController;

	virtual void NativeInitializeAnimation() override;

	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;
};
