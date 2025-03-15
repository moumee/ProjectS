// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SuraPawnPlayerStates.h"
#include "Components/ActorComponent.h"
#include "SuraPawnPlayerMovementComponent.generated.h"


class UCapsuleComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SURAS_API USuraPawnPlayerMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USuraPawnPlayerMovementComponent();
	
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetMoveInput(const FVector2D& InputVector);

	void SetJumpInput(bool bPressed);

	void SetDashInput(bool bPressed);

	void SetCrouchInput(bool bPressed);

	void ShowDebugInfo();

protected:

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	EPrimaryState PrimaryState;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	EGroundedState GroundedState;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	EAirborneState AirborneState;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	ECrouchState CrouchState;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float GravityScale = 980.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float JumpForce = 700.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MaxFallSpeed = 4000.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float WalkSpeed = 600.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float SprintSpeed = 1000.f;
	
	UPROPERTY(EditAnywhere, Category = "Movement")
	float Acceleration = 8000.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float Deceleration = 8000.f;

	UPROPERTY()
	APawn* PlayerPawn = nullptr;

	UPROPERTY()
	UCapsuleComponent* CapsuleComponent = nullptr;

	FVector2D MovementInput;

	FVector MovementInputDirection;
	
	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FVector Velocity;
	
	FVector Gravity;
	FVector GravityDirection;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bIsGrounded = false;

	// Manage dash as boolean because it can be either Grounded or Airborne
	bool bIsDashing = false;

	bool bJumpPressed = false;
	bool bDashPressed = false;
	bool bCrouchPressed = false;

	void CheckIsGrounded();
	
	void HandleState(float DeltaTime);

	void ApplyVelocity(float DeltaTime);

	void HandleGrounded(float DeltaTime);
	void HandleIdle(float DeltaTime);
	void HandleWalking(float DeltaTime);
	void HandleSprinting(float DeltaTime);
	void HandleCrouching(float DeltaTime);
	void HandleSliding(float DeltaTime);
	
	void HandleAirborne(float DeltaTime);
	void HandleFalling(float DeltaTime);
	void HandleJumping(float DeltaTime);
	void HandleDoubleJumping(float DeltaTime);
	
	void HandleWallRunning(float DeltaTime);
	
	void HandleMantling(float DeltaTime);
	
	void HandleHanging(float DeltaTime);

	FVector CollideAndSlide(const FVector& StartPos, const FVector& DeltaVelocity, int32 Depth);	
};


