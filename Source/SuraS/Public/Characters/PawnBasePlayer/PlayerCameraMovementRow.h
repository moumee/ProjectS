#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "PlayerCameraMovementRow.generated.h"

USTRUCT(BlueprintType)
struct SURAS_API FPlayerCameraMovementRow : public FTableRowBase
{
	GENERATED_BODY()

	// Loops
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
	
};