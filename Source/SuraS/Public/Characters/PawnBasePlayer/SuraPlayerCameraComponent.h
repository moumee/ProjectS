// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SuraPlayerCameraComponent.generated.h"


class UCameraComponent;
class USuraPlayerMovementComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SURAS_API USuraPlayerCameraComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USuraPlayerCameraComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

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
	TSubclassOf<UCameraShakeBase> DashCameraShake;

	UPROPERTY(VisibleAnywhere, Category="Camera Shake")
	TSubclassOf<UCameraShakeBase> CurrentLoopShake;
	

	void ChangeCameraLoopShake(const TSubclassOf<UCameraShakeBase>& InShake);
	void PlayOneShotCameraShake(const TSubclassOf<UCameraShakeBase>& InShake);
	void TickMoveStateLoopShake();

	void OnAirborne();
	void OnMove();
	void OnWallRun();
	void OnSlide();


	void OnLand(float ZVelocity);
	void OnPrimaryJump();
	void OnDoubleJump();
	void OnWallJump();
	void OnMantle();
	void OnDash();

	void InitCameraShakes();
};


