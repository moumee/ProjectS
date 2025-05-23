// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SuraPlayerEnums.h"
#include "ActorComponents/UISystem/ACUIMangerComponent.h"
#include "Characters/SuraCharacterBase.h"
#include "Interfaces/Damageable.h"
#include "Widgets/Player/PlayerHitWidget.h"
#include "SuraCharacterPlayer.generated.h"

class UACDamageSystem;
class USuraPlayerSlidingState;
class USphereComponent;
class USuraPlayerWallRunningState;
class USuraPlayerMantlingState;
class USuraPlayerHangingState;
class USuraPlayerCrouchingState;
class USuraPlayerDashingState;
class USuraPlayerFallingState;
class USuraPlayerJumpingState;
class USuraPlayerRunningState;
class USuraPlayerWalkingState;
class USuraPlayerBaseState;
class UACPlayerMovementData;
class UCameraComponent;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
class UWeaponSystemComponent;


/**
 * 
 */
UCLASS()
class SURAS_API ASuraCharacterPlayer : public ASuraCharacterBase, public IDamageable
{
	GENERATED_BODY()

	TSubclassOf<class UUserWidget> HitEffectWidgetClass;
	UPlayerHitWidget* HitEffectWidget;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponSystem")
	UWeaponSystemComponent* WeaponSystem;



	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
	UACDamageSystem* DamageSystemComponent;

	UPROPERTY()
	USuraPlayerBaseState* CurrentState;

	UPROPERTY()
	USuraPlayerBaseState* PreviousState;

	UPROPERTY()
	USuraPlayerBaseState* PreviousGroundedState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes")
	UACPlayerMovementData* PlayerMovementData;

	// UI component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseUI", meta = (AllowPrivateAccess = "true"))
	UACUIMangerComponent* UIManager;


#pragma region Input
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* DefaultMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* MoveAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* LookAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* JumpAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* ShootAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* CrouchAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* DashAction;


	

#pragma endregion Input

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug")
	bool bIsDebugMode;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	USkeletalMeshComponent* ArmMesh;

	float DefaultCapsuleHalfHeight;

	FVector DefaultCameraLocation;
	
	float BaseMovementSpeed;

	float AdditionalMovementSpeed;

	bool bShouldUpdateAdditionalMovementSpeed;

	float CurrentDashCooldownIndex = 0.f;

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// Adds on screen debug messages for debugging purposes
	void PrintPlayerDebugInfo() const;

	void UpdateDashCooldowns(float DeltaTime);
	
	void OnDamaged();

	void OnDeath();

	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;


#pragma region Input Action Callback Functions
	
	void Move(const FInputActionValue& InputValue);

	void StopMoving();

	void Look(const FInputActionValue& InputValue);
	
	void StartJumping();

	void StartCrouching();

	void StopCrouching();

	void StartDashing();

#pragma endregion Input Action Callback Functions

	virtual void Landed(const FHitResult& Hit) override;


public:
	ASuraCharacterPlayer();

	// Returns Player Movement Data Actor Component
	UFUNCTION(BlueprintCallable)
	UACPlayerMovementData* GetPlayerMovementData() const { return PlayerMovementData; }

	UCameraComponent* GetCamera() const { return Camera; }

	// Returns the angle of the current floor
	// If floor angle is 45 degrees downward slope, returns -45.f
	float FindFloorAngle() const;
	void RestoreCameraTilt(float DeltaTime);
	virtual void InterpCameraFOV(float DeltaTime);

	UPROPERTY(EditDefaultsOnly, Category = Camera)
	TSubclassOf<UCameraShakeBase> IdleCamShake;
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	TSubclassOf<UCameraShakeBase> WalkCamShake;
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	TSubclassOf<UCameraShakeBase> WallRunCamShake;
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	TSubclassOf<UCameraShakeBase> RunCamShake;
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	TSubclassOf<UCameraShakeBase> PrimaryJumpCamShake;
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	TSubclassOf<UCameraShakeBase> DoubleJumpCamShake;
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	TSubclassOf<UCameraShakeBase> DefaultLandCamShake;
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	TSubclassOf<UCameraShakeBase> MiddleLandCamShake;
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	TSubclassOf<UCameraShakeBase> HighLandCamShake;
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	TSubclassOf<UCameraShakeBase> SlideCamShake;
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	TSubclassOf<UCameraShakeBase> CrouchCamShake;
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	TSubclassOf<UCameraShakeBase> DashCamShake;
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	TSubclassOf<UCameraShakeBase> MantleCamShake;
	UPROPERTY(EditDefaultsOnly, Category = Camera)
	TSubclassOf<UCameraShakeBase> HangingCamShake;


	// It is used to change state to desired ground state when transitioning from Falling State
	UPROPERTY()
	USuraPlayerBaseState* DesiredGroundState;
	
	UPROPERTY(BlueprintReadOnly)
	USuraPlayerWalkingState* WalkingState;
	UPROPERTY(BlueprintReadOnly)
	USuraPlayerRunningState* RunningState;
	UPROPERTY(BlueprintReadOnly)
	USuraPlayerJumpingState* JumpingState;
	UPROPERTY(BlueprintReadOnly)
	USuraPlayerFallingState* FallingState;
	UPROPERTY(BlueprintReadOnly)
	USuraPlayerDashingState* DashingState;
	UPROPERTY(BlueprintReadOnly)
	USuraPlayerCrouchingState* CrouchingState;
	UPROPERTY(BlueprintReadOnly)
	USuraPlayerHangingState* HangingState;
	UPROPERTY(BlueprintReadOnly)
	USuraPlayerMantlingState* MantlingState;
	UPROPERTY(BlueprintReadOnly)
	USuraPlayerWallRunningState* WallRunningState;
	UPROPERTY(BlueprintReadOnly)
	USuraPlayerSlidingState* SlidingState;

	int MaxJumps = 2;
	int JumpsLeft;

	// Input Action Bound Boolean Flags
	bool bJumpTriggered = false;
	bool bDashTriggered = false;
	bool bCrouchTriggered = false;
	bool bLandedTriggered = false;

	void ResetTriggeredBooleans();

	FHitResult LedgeHitResult;
	FHitResult WallHitResult;

	EWallSide WallRunSide = EWallSide::None;
	FVector WallRunDirection = FVector::ZeroVector;

	int MaxDashes;
	int DashesLeft;

	FTimerHandle DashingTimerHandle;

	TArray<float> DashCooldowns;

	FTimerHandle AdditionalSpeedTimerHandle;

	// Player Input Axis Values
	// W - Sets ForwardAxisInputValue to 1
	// S - Sets ForwardAxisInputValue to -1
	// A - Sets RightAxisInputValue to -1
	// D - Sets RightAxisInputValue to 1
	float ForwardAxisInputValue;
	float RightAxisInputValue;

	float DefaultGroundFriction;
	float DefaultGravityScale;
	float DefaultBrakingDecelerationWalking;
	float DefaultBrakingDecelerationFalling;
	float DefaultBrakingFriction;
	float DefaultFallingLateralFriction;
	float DefaultCameraFOV;
	

	bool bShouldRestoreCameraTilt = false;
	bool bShouldRestoreCapsuleHalfHeight = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Movement)
	float XYSpeed;
	
	UFUNCTION(BlueprintCallable)
	void SetBaseMovementSpeed(float MovementSpeed);

	float GetBaseMovementSpeed() const;

	void ChangeState(USuraPlayerBaseState* NewState);

	bool IsFallingDown() const;

	bool HasWeapon() const;

	bool HasMovementInput() const;

	void PrimaryJump();

	void InAirJump();

	float GetDefaultCapsuleHalfHeight() const;

	FVector GetDefaultCameraLocation() const;

	USuraPlayerBaseState* GetCurrentState() const;

	USuraPlayerBaseState* GetPreviousState() const;

	USuraPlayerBaseState* GetPreviousGroundedState() const;

	void SetPreviousGroundedState(USuraPlayerBaseState* InState);

	bool IsDebugMode() const { return bIsDebugMode; }

	bool ShouldEnterWallRunning(FVector& OutWallRunDirection, EWallSide& OutWallRunSide);

	void InterpCapsuleHeight(float TargetScale, float DeltaTime);

	UACDamageSystem* GetDamageSystemComponent() const { return DamageSystemComponent; }
	
	virtual bool TakeDamage(const FDamageData& DamageData, const AActor* DamageCauser) override;

	virtual void StartCamShake(TSubclassOf<UCameraShakeBase> InShakeClass);

	UWeaponSystemComponent* GetWeaponSystemComponent() const { return WeaponSystem; }

};




