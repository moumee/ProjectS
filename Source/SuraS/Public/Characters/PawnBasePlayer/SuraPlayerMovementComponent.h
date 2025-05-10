// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "SuraPlayerMovementComponent.generated.h"

class ASuraPlayerController;
class ASuraPawnPlayer;

UENUM(Blueprintable)
enum class EWallRunEnter : uint8
{
	EWRE_Upward,
	EWRE_Downward,
	EWRE_Neutral
};

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

	void SetDefaultCapsuleValues(float Radius, float HalfHeight)
	{
		DefaultCapsuleRadius = Radius;
		DefaultCapsuleHalfHeight = HalfHeight;
	}

protected:

	UPROPERTY(EditAnywhere, Category = "Movement|Capsule")
	float DefaultCapsuleRadius;

	UPROPERTY(EditAnywhere, Category = "Movement|Capsule")
	float DefaultCapsuleHalfHeight;

	UPROPERTY(EditAnywhere, Category = "Movement|Capsule")
	float CrouchCapsuleHalfHeight = 40.f;

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

	UPROPERTY(EditAnywhere, Category = "Movement|Crouch")
	float CrouchHeightScale = 0.5f;

	UPROPERTY(EditAnywhere, Category = "Movement|Jump")
	float JumpHeight = 200.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float Acceleration = 8000.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float Deceleration = 8000.f;
	
	UPROPERTY(EditAnywhere, Category = "Movement|Air")
	float AirAcceleration = 2000.f;

	UPROPERTY(EditAnywhere, Category = "Movement|Air")
	float AirDeceleration = 2000.f;

	UPROPERTY(EditAnywhere, Category = "Movement|Air")
	float MaxFallVerticalSpeed = 3000.f;

	UPROPERTY(EditAnywhere, Category = "Movement|Walk")
	float MaxWalkableFloorAngle = 50.f;

	UPROPERTY(EditAnywhere, Category = "Movement|Walk")
	float MaxStepHeight = 30.f;

	UPROPERTY(EditAnywhere, Category = "Movement|WallRun")
	float WallRunMaxDuration = 2.f;

	UPROPERTY(EditAnywhere, Category = "Movement|WallRun")
	float WallRunAcceleration = 4000.f;
	
	UPROPERTY(EditAnywhere, Category = "Movement|WallRun")
	float WallRunDeceleration = 2000.f;

	UPROPERTY(EditAnywhere, Category = "Movement|WallRun")
	float WallRunMaxSpeed = 1200.f;

	UPROPERTY(EditAnywhere, Category = "Movement|WallRun")
	float WallRunBackwardMaxSpeed = 100.f;

	UPROPERTY(EditAnywhere, Category = "Movement|WallRun")
	float WallRunJumpAirSpeed2D = 1400.f;

	UPROPERTY(EditAnywhere, Category = "Movement|Slide")
	float SlideInitialWindow = 0.65f;

	UPROPERTY(EditAnywhere, Category = "Movement|Slide")
	float SlideMaxDuration = 1.25f;

protected:

	UPROPERTY(VisibleAnywhere, Category = "Movement")
	bool bIsCrouching = false;

	FHitResult GroundHit;

#pragma region WallRun
	
	EWallRunEnter WallRunEnterMode;
	bool bWallJumpAirBoost = false;
	bool bIsDeceleratingZ = false;
	UPROPERTY(VisibleAnywhere, Category = "Movement|WallRun")
	float WallRunElapsedTime = 0.f;
	float WallRunEnterSpeed2D;
	float WallJumpBuffer = 0.5f;
	EWallRunSide CurrentWallRunSide = EWallRunSide::EWRS_None;
	FHitResult CurrentWallHit;
	bool bShouldMoveUpWall = false;
	bool bShouldMoveDownWall = false;
	
#pragma endregion WallRun

#pragma region Dash
	
	UPROPERTY(VisibleAnywhere, Category = "Movement|Dash")
	bool bIsDashing = false;
	UPROPERTY(VisibleAnywhere, Category = "Movement|Dash")
	TArray<float> DashCooldowns;
	UPROPERTY(VisibleAnywhere, Category = "Movement|Dash")
	int32 AvailableDashCount = 2;
	UPROPERTY(VisibleAnywhere, Category = "Movement|Dash")
	float ElapsedTimeFromDash = 0.f;

#pragma endregion Dash

#pragma region Jump
	
	float JumpZVelocity = 0.f;
	float JumpBuffer = 0.1f;
	UPROPERTY(VisibleAnywhere, Category = "Movement|Jump")
	float ElapsedTimeFromSurface = 0.f;
	int32 MaxJumpCount = 2;
	UPROPERTY(VisibleAnywhere, Category = "Movement|Jump")
	int32 CurrentJumpCount = 0;

#pragma endregion Jump

#pragma region Slide

	// Used for determining when to lerp slide direction.
	// Reset to 0 whenever exiting slide state
	UPROPERTY(VisibleAnywhere, Category = "Movement|Slide")
	float SlideStateElapsedTime = 0.f;

	// Used for max slide time cap
	UPROPERTY(VisibleAnywhere, Category = "Movement|Slide")
	float SlideElapsedTime = 0.f;

	UPROPERTY(VisibleAnywhere, Category = "Movement|Slide")
	float SlideResetTimer = 0.f;

	float SlideBoostResetDelay = 0.35f;

	UPROPERTY(VisibleAnywhere, Category = "Movement|Slide")
	float SlideDecelerationAmount = 1600.f;

	FVector SlideStartDirection;

	UPROPERTY(VisibleAnywhere, Category = "Movement|Slide")
	bool bHasRecentlySlid = false;

	
#pragma endregion Slide

#pragma region Mantle

	FHitResult MantleWallHit;

	FHitResult MantleFloorHit;

	
#pragma endregion Mantle

	UPROPERTY()
	TObjectPtr<ASuraPawnPlayer> SuraPawnPlayer = nullptr;

	UPROPERTY()
	TObjectPtr<ASuraPlayerController> SuraPlayerController = nullptr;

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

	bool IsGrounded();

	void SetMovementState(EMovementState NewState);

	void OnMovementStateChanged(EMovementState OldState, EMovementState NewState);

	void TickState(float DeltaTime);
	
	void CrouchCapsule(float DeltaTime);

	void UnCrouchCapsule(float DeltaTime);

	void TickMove(float DeltaTime);

	void TickSlide(float DeltaTime);
	
	bool CanWallRun();

	void TickAirborne(float DeltaTime);

	void TickWallRun(float DeltaTime);

	void TickMantle(float DeltaTime);

	void TickHang(float DeltaTime);

	void UpdateDashCooldowns(float DeltaTime);

};
