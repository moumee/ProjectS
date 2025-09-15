// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "Enumerations/MovementTriggerEnums.h"
#include "SuraPlayerMovementTrigger.generated.h"



USTRUCT(BlueprintType)
struct FPlayerKeyHoldModifier
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	EMovementTriggerKey Key = EMovementTriggerKey::Ctrl;
};

USTRUCT(BlueprintType)
struct FPlayerMovementDataModifier
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	EMovementDataType DataType = EMovementDataType::RunSpeed;

	UPROPERTY(EditAnywhere)
	float ModifiedValue = 0.f;
	
};


UCLASS()
class SURAS_API ASuraPlayerMovementTrigger : public ATriggerBox
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASuraPlayerMovementTrigger();

protected:

	UPROPERTY(EditAnywhere)
	EMovementTriggerType MovementTriggerType;

	UPROPERTY(EditAnywhere, meta = (EditCondition = "MovementTriggerType == EMovementTriggerType::Start", EditConditionHides))
	TArray<FPlayerKeyHoldModifier> PlayerKeyHoldModifiers;
	UPROPERTY(EditAnywhere, meta = (EditCondition = "MovementTriggerType == EMovementTriggerType::Start", EditConditionHides))
	TArray<FPlayerMovementDataModifier> PlayerMovementDataModifiers;
	
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnTriggerBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

};
