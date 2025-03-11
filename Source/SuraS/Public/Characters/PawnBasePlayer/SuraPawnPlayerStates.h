#pragma once

UENUM(BlueprintType)
enum class EPrimaryState : uint8
{
	Grounded	UMETA(DisplayName = "Grounded"),
	Airborne	UMETA(DisplayName = "Airbone"),
	WallRunning	UMETA(DisplayName = "WallRunning"),
	Mantling	UMETA(DisplayName = "Mantling"),
	Hanging		UMETA(DisplayName = "Hanging"),
};

UENUM(BlueprintType)
enum class EGroundedState : uint8
{
	Idle		UMETA(DisplayName = "Idle"),
	Walking		UMETA(DisplayName = "Walking"),
	Sprinting	UMETA(DisplayName = "Sprinting"),
	Crouching	UMETA(DisplayName = "Crouching"),
	Sliding		UMETA(DisplayName = "Sliding"),
};

UENUM(BlueprintType)
enum class EAirborneState : uint8
{
	Falling	UMETA(DisplayName = "Falling"),
	Jumping	UMETA(DisplayName = "Jumping"),
	DoubleJumping	UMETA(DisplayName = "DoubleJumping"),
};

UENUM(BlueprintType)
enum class ECrouchState : uint8
{
	NotCrouching	UMETA(DisplayName = "NotCrouching"),
	Crouching		UMETA(DisplayName = "Crouching"),
};