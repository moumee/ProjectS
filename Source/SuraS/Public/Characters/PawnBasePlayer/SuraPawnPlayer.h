// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SuraPawnPlayer.generated.h"

class UCameraComponent;
class UCapsuleComponent;
class USuraPawnPlayerMovementComponent;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;

UCLASS()
class SURAS_API ASuraPawnPlayer : public APawn
{
	GENERATED_BODY()

public:
	ASuraPawnPlayer();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual void BeginPlay() override;

protected:

	UPROPERTY(EditAnywhere)
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(EditAnywhere, Category = "Blueprint Assign")
	USkeletalMeshComponent* ArmMesh;

	UPROPERTY(EditAnywhere)
	UCameraComponent* Camera;

	UPROPERTY()
	USuraPawnPlayerMovementComponent* MovementComponent;

	UPROPERTY(EditAnywhere, Category = "Blueprint Assign")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, Category = "Blueprint Assign")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "Blueprint Assign")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = "Blueprint Assign")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = "Blueprint Assign")
	UInputAction* DashAction;

	UPROPERTY(EditAnywhere, Category = "Blueprint Assign")
	UInputAction* CrouchAction;

	void StartMove(const FInputActionValue& Value);
	void StopMove();

	void Look(const FInputActionValue& Value);

};


