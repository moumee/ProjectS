// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "SuraPlayerMovementComponent.generated.h"

class ASuraPawnPlayer;

UENUM(Blueprintable)
enum class EMovementState : uint8
{
	EMS_Move,
	EMS_Slide,
	EMS_Airborne,
	EMS_Dash,
	EMS_WallRun,
	EMS_Mantle,
	EMS_Hang
};


UCLASS()
class SURAS_API USuraPlayerMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()

public:

	USuraPlayerMovementComponent();

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetMovementInputVector(const FVector2D& InMovementInputVector);

	void SetJumpPressed(bool bPressed);

	void SetDashPressed(bool bPressed);

	void SetCrouchPressed(bool bPressed);

protected:

	UPROPERTY(EditAnywhere, Category = "Movement")
	float GravityScale = 980.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float WalkSpeed = 600.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float RunSpeed = 1000.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float CrouchSpeed = 400.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float JumpHeight = 300.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float Acceleration = 1000.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float AirAcceleration = 500.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float Deceleration = 2000.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MaxFallVerticalSpeed = 3000.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MaxWalkableFloorAngle = 50.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MaxStepHeight = 50.f;

protected:

	UPROPERTY()
	ASuraPawnPlayer* SuraPawnPlayer = nullptr;

	FVector GravityAcceleration;
	
	FVector GravityDirection = FVector::DownVector;

	EMovementState PreviousMovementState;
	
	EMovementState CurrentMovementState;
	
	FVector2D MovementInputVector = FVector2D::ZeroVector;

	float MinWalkableFloorZ;

	bool bJumpPressed = false;

	bool bDashPressed = false;

	bool bCrouchPressed = false;

	bool bIsGrounded = true;

	bool IsGrounded() const;

	void SetMovementState(EMovementState NewState);

	void OnMovementStateChanged(EMovementState NewState);

	void TickState(float DeltaTime);

	void TickMove(float DeltaTime);

	void TickSlide(float DeltaTime);

	void TickAirborne(float DeltaTime);

	void TickDash(float DeltaTime);

	void TickWallRun(float DeltaTime);

	void TickMantle(float DeltaTime);

	void TickHang(float DeltaTime);

};
