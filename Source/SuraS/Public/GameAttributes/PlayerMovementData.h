// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerMovementData.generated.h"

/**
 *
 */
USTRUCT(BlueprintType)
struct SURAS_API FPlayerMovementData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "중력 스케일"))
	float GravityScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "걷기 속력"))
	float WalkSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "걷기/달리기 속력 전환 시간"))
	float WalkRunTransitionDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "걷기/대쉬 속력 전환 시간"))
	float WalkDashTransitionDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "달리기/대쉬 속력 전환 시간"))
	float RunDashTransitionDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "걷기/앉기 속력 전환 시간"))
	float WalkCrouchTransitionDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "앉기/달리기 속력 전환 시간"))
	float CrouchRunTransitionDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "앉기/대쉬 속력 전환 시간"))
	float CrouchDashTransitionDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DashSpeedDecreaseDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "달리기 속력"))
	float RunSpeed;

	// The Z value of the launch velocity when jumping
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "일반 점프 Z축 속력"))
	float PrimaryJumpZSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "더블 점프 Z축 속력"))
	float DoubleJumpZSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "더블 점프 XY평면 속력"))
	float DoubleJumpXYSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "1.0", ToolTip = "공중 컨트롤"))
	float AirControl;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "앉기 속력"))
	float CrouchSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "대쉬 후 속력"))
	float DashEndSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "대쉬 속력"))
	float DashImpulseSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0"))
	int DashMaxStack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0"))
	float DashCooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "슬라이딩 추가 속력"))
	float SlidingAdditionalSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "슬라이딩 속력 증가 시간"))
	float SlidingSpeedIncreaseTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "슬라이딩 속력 감소 시간"))
	float SlidingSpeedDecreaseTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WallRunningMaxDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxCameraFOV;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxCameraFOVSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxWallRunSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WallRunEnterZVelocityThreshold;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WallRunEnterVerticalForce;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WallRunCameraTiltAngle;
	
};
