// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Pawn.h"
#include "SuraPawnPlayer.generated.h"

class USuraPlayerCameraComponent;
class USpringArmComponent;
class USuraPlayerMovementComponent;
class UWeaponSystemComponent;
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

	virtual void PossessedBy(AController* NewController) override;
	
	virtual void BeginPlay() override;

	UCapsuleComponent* GetCapsuleComponent();

	UCameraComponent* GetCameraComponent() const { return Camera; };

	UWeaponSystemComponent* GetWeaponSystemComponent() const { return WeaponSystem; }  // <WeaponSystem>

	USkeletalMeshComponent* GetArmMesh() { return ArmMesh; }  // <WeaponSystem>

	bool HasWeapon() const;  // <WeaponSystem>s

	void UpdateLookInputVector2D(const FInputActionValue& InputValue);  // <WeaponSystem>
	void SetLookInputVector2DZero();  // <WeaponSystem>
	FVector2D GetPlayerLookInputVector() const { return PlayerLookInputVector2D; } // <WeaponSystem>
	USuraPlayerMovementComponent* GetPlayerMovementComponent() { return MovementComponent; };

protected:

	UPROPERTY(EditAnywhere)
	TObjectPtr<UCapsuleComponent> CapsuleComponent;

	UPROPERTY(EditAnywhere, Category = "Blueprint Assign")
	TObjectPtr<USkeletalMeshComponent> ArmMesh;

	UPROPERTY(EditAnywhere, Category = "Blueprint Assign")
	TObjectPtr<USkeletalMeshComponent> HandsMesh;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USuraPlayerMovementComponent> MovementComponent;

	UPROPERTY(EditAnywhere, Category = "WeaponSystem")
	TObjectPtr<UWeaponSystemComponent> WeaponSystem;  // <WeaponSystem>
	// This actor component is for handling camera shakes and state based movement
	// IT IS NOT THE CAMERA!!
	UPROPERTY(EditAnywhere)
	TObjectPtr<USuraPlayerCameraComponent> CameraMovementComponent;

	UPROPERTY(EditAnywhere, Category = "Blueprint Assign")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, Category = "Blueprint Assign")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = "Blueprint Assign")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, Category = "Blueprint Assign")
	TObjectPtr<UInputAction> RunAction;

	UPROPERTY(EditAnywhere, Category = "Blueprint Assign")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, Category = "Blueprint Assign")
	TObjectPtr<UInputAction> DashAction;

	UPROPERTY(EditAnywhere, Category = "Blueprint Assign")
	TObjectPtr<UInputAction> CrouchAction;
	

	FVector2D PlayerLookInputVector2D; // <WeaponSystem>

	void HandleMoveInput(const FInputActionValue& Value);
	void HandleLookInput(const FInputActionValue& Value);
	void StartJumpInput();
	void StopJumpInput();
	void StartDashInput();
	void StopDashInput();
	void StartCrouchInput();
	void StopCrouchInput();

};


