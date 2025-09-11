// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerCameraMovementRow.h"
#include "Animations/CameraAnimationCameraModifier.h"
#include "Components/ActorComponent.h"
#include "SuraPlayerCameraComponent.generated.h"


class UTimelineComponent;
class UCameraComponent;
class USuraPlayerMovementComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SURAS_API USuraPlayerCameraComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USuraPlayerCameraComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void OnDowned();

protected:
	virtual void BeginPlay() override;

	// Move state means that the player is grounded and not sliding
	bool bIsMoveState = false;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UDataTable> CameraDataTable;

	UPROPERTY(VisibleDefaultsOnly)
    TObjectPtr<USuraPlayerMovementComponent> MovementComponent;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCameraComponent> PlayerCamera;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<APlayerController> PlayerController;

	UPROPERTY(EditDefaultsOnly, Category="Camera Transition")
	FMovementCameraData IdleCameraData;
	UPROPERTY(EditDefaultsOnly, Category="Camera Transition")
	FMovementCameraData WalkHorizontalCameraData;
	UPROPERTY(EditDefaultsOnly, Category="Camera Transition")
	FMovementCameraData WalkBackwardCameraData;
	UPROPERTY(EditDefaultsOnly, Category="Camera Transition")
	FMovementCameraData RunCameraData;
	UPROPERTY(EditDefaultsOnly, Category="Camera Transition")
	FMovementCameraData WallRunCameraData;
	UPROPERTY(EditDefaultsOnly, Category="Camera Transition")
	FMovementCameraData SlideCameraData;
	UPROPERTY(EditDefaultsOnly, Category="Camera Transition")
	FMovementCameraData FallCameraData;
	UPROPERTY(EditDefaultsOnly, Category="Camera Transition")
	FMovementCameraData CrouchCameraData;
	
	// Loop cam shakes
	UPROPERTY(EditDefaultsOnly, Category="Camera Shake|Loop")
	TSubclassOf<UCameraShakeBase> IdleCameraShake;
	UPROPERTY(EditDefaultsOnly, Category="Camera Shake|Loop")
	TSubclassOf<UCameraShakeBase> WalkCameraShake;
	UPROPERTY(EditDefaultsOnly, Category="Camera Shake|Loop")
	TSubclassOf<UCameraShakeBase> RunCameraShake;
	UPROPERTY(EditDefaultsOnly, Category="Camera Shake|Loop")
	TSubclassOf<UCameraShakeBase> CrouchIdleCameraShake;
	UPROPERTY(EditDefaultsOnly, Category="Camera Shake|Loop")
	TSubclassOf<UCameraShakeBase> CrouchMoveCameraShake;
	UPROPERTY(EditDefaultsOnly, Category="Camera Shake|Loop")
	TSubclassOf<UCameraShakeBase> AirborneCameraShake;
	UPROPERTY(EditDefaultsOnly, Category="Camera Shake|Loop")
	TSubclassOf<UCameraShakeBase> WallRunCameraShake;
	UPROPERTY(EditDefaultsOnly, Category="Camera Shake|Loop")
	TSubclassOf<UCameraShakeBase> SlideCameraShake;


	// One shot cam shakes
	UPROPERTY(EditDefaultsOnly, Category="Camera Shake|One Shot")
	TSubclassOf<UCameraShakeBase> PrimaryJumpCameraShake;
	UPROPERTY(EditDefaultsOnly, Category="Camera Shake|One Shot")
	TSubclassOf<UCameraShakeBase> DoubleJumpCameraShake;
	UPROPERTY(EditDefaultsOnly, Category="Camera Shake|One Shot")
	TSubclassOf<UCameraShakeBase> WallJumpCameraShake;
	UPROPERTY(EditDefaultsOnly, Category="Camera Shake|One Shot")
	TSubclassOf<UCameraShakeBase> LightLandCameraShake;
	UPROPERTY(EditDefaultsOnly, Category="Camera Shake|One Shot")
	TSubclassOf<UCameraShakeBase> MediumLandCameraShake;
	UPROPERTY(EditDefaultsOnly, Category="Camera Shake|One Shot")
	TSubclassOf<UCameraShakeBase> StrongLandCameraShake;
	UPROPERTY(EditDefaultsOnly, Category="Camera Shake|One Shot")
	TSubclassOf<UCameraShakeBase> WallLandCameraShake;
	UPROPERTY(EditDefaultsOnly, Category="Camera Shake|One Shot")
	TSubclassOf<UCameraShakeBase> MantleCameraShake;
	UPROPERTY(EditDefaultsOnly, Category="Camera Shake|One Shot")
	TSubclassOf<UCameraShakeBase> ForwardDashCameraShake;
	UPROPERTY(EditDefaultsOnly, Category="Camera Shake|One Shot")
	TSubclassOf<UCameraShakeBase> ForwardRightDashCameraShake;
	UPROPERTY(EditDefaultsOnly, Category="Camera Shake|One Shot")
	TSubclassOf<UCameraShakeBase> ForwardLeftDashCameraShake;
	UPROPERTY(EditDefaultsOnly, Category="Camera Shake|One Shot")
	TSubclassOf<UCameraShakeBase> RightDashCameraShake;
	UPROPERTY(EditDefaultsOnly, Category="Camera Shake|One Shot")
	TSubclassOf<UCameraShakeBase> LeftDashCameraShake;
	UPROPERTY(EditDefaultsOnly, Category="Camera Shake|One Shot")
	TSubclassOf<UCameraShakeBase> BackwardDashCameraShake;
	UPROPERTY(EditDefaultsOnly, Category="Camera Shake|One Shot")
	TSubclassOf<UCameraShakeBase> BackwardRightDashCameraShake;
	UPROPERTY(EditDefaultsOnly, Category="Camera Shake|One Shot")
	TSubclassOf<UCameraShakeBase> BackwardLeftDashCameraShake;
	

	UPROPERTY(VisibleAnywhere, Category="Camera Shake")
	TSubclassOf<UCameraShakeBase> CurrentLoopShake;
	
	UPROPERTY(EditDefaultsOnly, Category="Downed")
	TSubclassOf<UCameraShakeBase> DownedImpactShake;
	UPROPERTY(EditDefaultsOnly, Category="Downed")
	TSubclassOf<UCameraShakeBase> DownedFallShake;
	UPROPERTY(EditDefaultsOnly, Category="Downed")
	TSubclassOf<UCameraShakeBase> DownedFloorImpactShake;
	UPROPERTY(EditDefaultsOnly, Category="Downed")
	TSubclassOf<UCameraShakeBase> DownedFloorIdleShake;
	UPROPERTY(EditDefaultsOnly, Category="Downed")
	TSubclassOf<UCameraShakeBase> DownedGoingUpShake;
	
	FTimerHandle DownFloorImpactTimerHandle;
	FTimerHandle DownGoingUpTimerHandle;
	
	FTimerDelegate DownedFloorImpactDelegate;
	FTimerDelegate DownedGoingUpShakeDelegate;

	void ChangeCameraLoopShake(const TSubclassOf<UCameraShakeBase>& InShake);
	void PlayOneShotCameraShake(const TSubclassOf<UCameraShakeBase>& InShake);
	void TickMoveStateCamera(float DeltaTime);
	void TickWallRunStateCamera(float DeltaTime);
	void TickSlideStateCamera(float DeltaTime);
	void TickAirborneStateCamera(float DeltaTime);

	void InterpCameraData(const FMovementCameraData& InData, float DeltaTime);

	void OnAirborne();
	void OnMove();
	void OnWallRun();
	void OnSlide();


	void OnLand(float ZVelocity);
	void OnPrimaryJump();
	void OnDoubleJump();
	void OnWallJump();
	void OnMantle();
	void OnDash(FVector2D MovementInput);
	

	void InitCameraShakes();
	
};


