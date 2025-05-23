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
	EMS_WallRun,
	EMS_Mantle,
	EMS_Hang
};

UENUM(Blueprintable)
enum class EWallRunSide : uint8
{
	EWRS_None,
	EWRS_Left,
	EWRS_Right,
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

	UPROPERTY(EditAnywhere, Category = "Movement|Gravity")
	float GravityScale = 1500.f;

	UPROPERTY(EditAnywhere, Category = "Movement|Walk")
	float WalkSpeed = 600.f;

	UPROPERTY(EditAnywhere, Category = "Movement|Dash")
	float DashStartSpeed = 3000.f;

	UPROPERTY(EditAnywhere, Category = "Movement|Dash")
	float DashEndSpeed = 1500.f;

	UPROPERTY(EditAnywhere, Category = "Movement|Dash")
	float DashDecelerationTime = 0.4f;

	UPROPERTY(EditAnywhere, Category = "Movement|Dash")
	float DashCooldown = 5.f;

	UPROPERTY(EditAnywhere, Category = "Movement|Run")
	float RunSpeed = 1000.f;
	
	UPROPERTY(EditAnywhere, Category = "Movement|Crouch")
	float CrouchSpeed = 400.f;

	UPROPERTY(EditAnywhere, Category = "Movement|Jump")
	float JumpHeight = 200.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float Acceleration = 8000.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float Deceleration = 8000.f;
	
	UPROPERTY(EditAnywhere, Category = "Movement|Air")
	float AirAcceleration = 1500.f;

	UPROPERTY(EditAnywhere, Category = "Movement|Air")
	float AirDeceleration = 2000.f;

	UPROPERTY(EditAnywhere, Category = "Movement|Air")
	float MaxFallVerticalSpeed = 3000.f;

	UPROPERTY(EditAnywhere, Category = "Movement|Walk")
	float MaxWalkableFloorAngle = 50.f;

	UPROPERTY(EditAnywhere, Category = "Movement|Walk")
	float MaxStepHeight = 30.f;

protected:

	EWallRunSide CurrentWallSide = EWallRunSide::EWRS_None;

	FHitResult LeftWallHit;

	FHitResult RightWallHit;
	
	UPROPERTY(VisibleAnywhere, Category = "Movement|Dash")
	bool bIsDashing = false;
	
	UPROPERTY(VisibleAnywhere, Category = "Movement|Dash")
	TArray<float> DashCooldowns;

	UPROPERTY(VisibleAnywhere, Category = "Movement|Dash")
	int32 AvailableDashCount = 2;

	float JumpZVelocity = 0.f;
	
	float JumpBuffer = 0.1f;

	UPROPERTY(VisibleAnywhere, Category = "Movement|Jump")
	float ElapsedTimeFromGround = 0.f;

	UPROPERTY(VisibleAnywhere, Category = "Movement|Dash")
	float ElapsedTimeFromDash = 0.f;

	int32 MaxJumpCount = 2;

	UPROPERTY(VisibleAnywhere, Category = "Movement|Jump")
	int32 CurrentJumpCount = 0;

	UPROPERTY()
	ASuraPawnPlayer* SuraPawnPlayer = nullptr;

	FVector GravityAcceleration;
	
	FVector GravityDirection = FVector::DownVector;

	UPROPERTY(VisibleAnywhere, Category = "Movement")
	EMovementState PreviousMovementState;

	UPROPERTY(VisibleAnywhere, Category = "Movement")
	EMovementState CurrentMovementState;

	UPROPERTY(VisibleAnywhere, Category = "Movement")
	FVector2D MovementInputVector = FVector2D::ZeroVector;

	float MinWalkableFloorZ;

	UPROPERTY(VisibleAnywhere, Category = "Movement|Jump")
	bool bJumpPressed = false;

	UPROPERTY(VisibleAnywhere, Category = "Movement|Dash")
	bool bDashPressed = false;

	UPROPERTY(VisibleAnywhere, Category = "Movement|Crouch")
	bool bCrouchPressed = false;

	bool IsGrounded() const;

	bool TryStepUp(const FHitResult& Hit, const FVector& DeltaMove, float DeltaTime);

	void SetMovementState(EMovementState NewState);

	void OnMovementStateChanged(EMovementState OldState, EMovementState NewState);

	void TickState(float DeltaTime);

	void TickMove(float DeltaTime);

	void TickSlide(float DeltaTime);

	void TickAirborne(float DeltaTime);

	void TickWallRun(float DeltaTime);

	void TickMantle(float DeltaTime);

	void TickHang(float DeltaTime);

	void UpdateDashCooldowns(float DeltaTime);

};
