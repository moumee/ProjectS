// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/ACPlayerMovmentData.h"

#include "GameAttributes/PlayerMovementData.h"

void UACPlayerMovementData::BeginPlay()
{
	Super::BeginPlay();

	PlayerRow = PlayerMovementDT->FindRow<FPlayerMovementData>(FName("Player"), FString(TEXT("")));
	
}

float UACPlayerMovementData::GetGravityScale() const
{
	if (!PlayerRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerRow is null"));
		return 0;
	}
	return PlayerRow->GravityScale;
}

float UACPlayerMovementData::GetWalkSpeed() const
{
	if (!PlayerRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerRow is null"));
		return 0;
	}
	return PlayerRow->WalkSpeed;
	
}

float UACPlayerMovementData::GetWalkRunTransitionDuration() const
{
	if (!PlayerRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerRow is null"));
		return 0;
	}
	return PlayerRow->WalkRunTransitionDuration;
}

float UACPlayerMovementData::GetWalkDashTransitionDuration() const
{
	if (!PlayerRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerRow is null"));
		return 0;
	}
	return PlayerRow->WalkDashTransitionDuration;
}

float UACPlayerMovementData::GetRunDashTransitionDuration() const
{
	if (!PlayerRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerRow is null"));
		return 0;
	}
	return PlayerRow->RunDashTransitionDuration;
}


float UACPlayerMovementData::GetCrouchRunTransitionDuration() const
{
	if (!PlayerRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerRow is null"));
		return 0;
	}
	return PlayerRow->CrouchRunTransitionDuration;
}

float UACPlayerMovementData::GetCrouchDashTransitionDuration() const
{
	if (!PlayerRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerRow is null"));
		return 0;
	}
	return PlayerRow->CrouchDashTransitionDuration;
}

float UACPlayerMovementData::GetWalkCrouchTransitionDuration() const
{
	if (!PlayerRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerRow is null"));
		return 0;
	}
	return PlayerRow->WalkCrouchTransitionDuration;
}


float UACPlayerMovementData::GetRunSpeed() const
{
	if (!PlayerRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerRow is null"));
		return 0;
	}
	return PlayerRow->RunSpeed;
}

float UACPlayerMovementData::GetCrouchSpeed() const
{
	if (!PlayerRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerRow is null"));
		return 0;
	}
	return PlayerRow->CrouchSpeed;
}

float UACPlayerMovementData::GetPrimaryJumpZSpeed() const
{
	if (!PlayerRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerRow is null"));
		return 0;
	}
	return PlayerRow->PrimaryJumpZSpeed;
}

float UACPlayerMovementData::GetDoubleJumpZSpeed() const
{
	if (!PlayerRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerRow is null"));
		return 0;
	}
	return PlayerRow->DoubleJumpZSpeed;
}

float UACPlayerMovementData::GetDoubleJumpXYSpeed() const
{
	if (!PlayerRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerRow is null"));
		return 0;
	}
	return PlayerRow->DoubleJumpXYSpeed;
}

float UACPlayerMovementData::GetAirControl() const
{
	if (!PlayerRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerRow is null"));
		return 0;
	}
	return PlayerRow->AirControl;
}

float UACPlayerMovementData::GetDashEndSpeed() const
{
	if (!PlayerRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerRow is null"));
		return 0;
	}
	return PlayerRow->DashEndSpeed;
}

float UACPlayerMovementData::GetDashImpulseSpeed() const
{
	if (!PlayerRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerRow is null"));
		return 0;
	}
	return PlayerRow->DashImpulseSpeed;
}

float UACPlayerMovementData::GetDashSpeedDecreaseDuration() const
{
	if (!PlayerRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerRow is null"));
		return 0;
	}
	return PlayerRow->DashSpeedDecreaseDuration;
}

int UACPlayerMovementData::GetDashMaxStack() const
{
	if (!PlayerRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerRow is null"));
		return 0;
	}
	return PlayerRow->DashMaxStack;
}

float UACPlayerMovementData::GetDashCooldown() const
{
	if (!PlayerRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerRow is null"));
		return 0;
	}
	return PlayerRow->DashCooldown;
}

float UACPlayerMovementData::GetSlidingAdditionalSpeed() const
{
	if (!PlayerRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerRow is null"));
		return 0;
	}
	return PlayerRow->SlidingAdditionalSpeed;
}

float UACPlayerMovementData::GetSlidingSpeedIncreaseTime() const
{
	if (!PlayerRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerRow is null"));
		return 0;
	}
	return PlayerRow->SlidingSpeedIncreaseTime;
}

float UACPlayerMovementData::GetSlidingSpeedDecreaseTime() const
{
	if (!PlayerRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerRow is null"));
		return 0;
	}
	return PlayerRow->SlidingSpeedDecreaseTime;
}



float UACPlayerMovementData::GetWallRunningMaxDuration() const
{
	if (!PlayerRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerRow is null"));
		return 0;
	}
	return PlayerRow->WallRunningMaxDuration;
}

float UACPlayerMovementData::GetMaxCameraFOV() const
{
	if (!PlayerRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerRow is null"));
		return 0;
	}
	return PlayerRow->MaxCameraFOV;
}

float UACPlayerMovementData::GetMaxCameraFOVSpeed() const
{
	if (!PlayerRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerRow is null"));
		return 0;
	}
	return PlayerRow->MaxCameraFOVSpeed;
}

float UACPlayerMovementData::GetMaxWallRunSpeed() const
{
	if (!PlayerRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerRow is null"));
		return 0;
	}
	return PlayerRow->MaxWallRunSpeed;
}

float UACPlayerMovementData::GetWallRunEnterZVelocityThreshold() const
{
	if (!PlayerRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerRow is null"));
		return 0;
	}
	return PlayerRow->WallRunEnterZVelocityThreshold;
}

float UACPlayerMovementData::GetWallRunEnterVerticalForce() const
{
	if (!PlayerRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerRow is null"));
		return 0;
	}
	return PlayerRow->WallRunEnterVerticalForce;
}

float UACPlayerMovementData::GetWallRunCameraTiltAngle() const
{
	if (!PlayerRow)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerRow is null"));
		return 0;
	}
	return PlayerRow->WallRunCameraTiltAngle;
}



