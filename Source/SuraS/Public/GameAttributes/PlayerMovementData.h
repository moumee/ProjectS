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
	float GravityScale = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "걷기 속력"))
	float WalkSpeed = 400.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "걷기/달리기 속력 전환 시간"))
	float WalkRunTransitionDuration = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "걷기/대쉬 속력 전환 시간"))
	float WalkDashTransitionDuration = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "달리기/대쉬 속력 전환 시간"))
	float RunDashTransitionDuration = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "걷기/앉기 속력 전환 시간"))
	float WalkCrouchTransitionDuration = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "앉기/달리기 속력 전환 시간"))
	float CrouchRunTransitionDuration = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "앉기/대쉬 속력 전환 시간"))
	float CrouchDashTransitionDuration = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DashSpeedDecreaseDuration = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "달리기 속력"))
	float RunSpeed = 700.f;

	// The Z value of the launch velocity when jumping
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "일반 점프 Z축 속력"))
	float PrimaryJumpZSpeed = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "더블 점프 Z축 속력"))
	float DoubleJumpZSpeed = 400.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "더블 점프 XY평면 속력"))
	float DoubleJumpXYSpeed = 700.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0", ClampMax = "1.0", ToolTip = "공중 컨트롤"))
	float AirControl = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "앉기 속력"))
	float CrouchSpeed = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "대쉬 후 속력"))
	float DashEndSpeed = 700.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "대쉬 속력"))
	float DashImpulseSpeed = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0"))
	int DashMaxStack = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0.0"))
	float DashCooldown = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "슬라이딩 추가 속력"))
	float SlidingAdditionalSpeed = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "슬라이딩 속력 증가 시간"))
	float SlidingSpeedIncreaseTime = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ToolTip = "슬라이딩 속력 감소 시간"))
	float SlidingSpeedDecreaseTime = 0.8f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WallRunningMaxDuration = 2.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxCameraFOV = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxCameraFOVSpeed = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxWallRunSpeed = 850.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WallRunEnterZVelocityThreshold = 200.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WallRunEnterVerticalForce = 300.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WallRunCameraTiltAngle = 15.f;
	
};
