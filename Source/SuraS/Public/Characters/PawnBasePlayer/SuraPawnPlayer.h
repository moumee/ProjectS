// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Pawn.h"
#include "SuraPawnPlayer.generated.h"

class USuraPlayerMovementComponent;
class UCameraComponent;
class UCapsuleComponent;
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

	UCapsuleComponent* GetCapsuleComponent();

	UCameraComponent* GetCameraComponent() const { return Camera; };

protected:

	UPROPERTY(EditAnywhere)
	TObjectPtr<UCapsuleComponent> CapsuleComponent;

	UPROPERTY(EditAnywhere, Category = "Blueprint Assign")
	TObjectPtr<USkeletalMeshComponent> ArmMesh;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USuraPlayerMovementComponent> MovementComponent;

	UPROPERTY(EditAnywhere, Category = "Blueprint Assign")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, Category = "Blueprint Assign")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = "Blueprint Assign")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, Category = "Blueprint Assign")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, Category = "Blueprint Assign")
	TObjectPtr<UInputAction> DashAction;

	UPROPERTY(EditAnywhere, Category = "Blueprint Assign")
	TObjectPtr<UInputAction> CrouchAction;

	void HandleMoveInput(const FInputActionValue& Value);
	void HandleLookInput(const FInputActionValue& Value);
	void StartJumpInput();
	void StopJumpInput();
	void StartDashInput();
	void StopDashInput();
	void StartCrouchInput();
	void StopCrouchInput();

};


