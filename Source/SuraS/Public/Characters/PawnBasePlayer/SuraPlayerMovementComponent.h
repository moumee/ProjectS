// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTags.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PawnMovementComponent.h"
#include "SuraPlayerMovementComponent.generated.h"

class ASuraPawnPlayer;
/**
 * 
 */
UCLASS()
class SURAS_API USuraPlayerMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()

public:

	USuraPlayerMovementComponent();

	virtual void BeginPlay() override;
	
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	const FGameplayTagContainer& GetOwnedGameplayTags() const { return OwnedGameplayTags; }

	void SetMovementInputVector(const FVector2D& InMovementInputVector);

	void SetJumpPressed(bool bPressed);

	void SetDashPressed(bool bPressed);

	void SetCrouchPressed(bool bPressed);

protected:

	UPROPERTY(EditAnywhere, Category = "Movement")
	float GravityScale = 980.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float WalkSpeed = 600.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float RunSpeed = 1000.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float CrouchSpeed = 400.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float JumpHeight = 300.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float Acceleration = 1000.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float Deceleration = 2000.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MaxWalkableFloorAngle = 50.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MaxStepHeight = 50.f;

protected:

	UPROPERTY()
	ASuraPawnPlayer* SuraPawnPlayer = nullptr;

	FVector Gravity = GravityDirection * GravityScale;
	
	FVector GravityDirection = FVector::DownVector;

	float WishSpeed = WalkSpeed;

	FVector2D MovementInputVector = FVector2D::ZeroVector;

	FGameplayTagContainer OwnedGameplayTags;

	FGameplayTag MovementStateTag;

	float MinWalkableFloorZ;

	bool bJumpPressed = false;

	bool bDashPressed = false;

	bool bCrouchPressed = false;

	bool bIsGrounded = true;

	bool IsGrounded() const;

	void SetMovementState(const FGameplayTag& InStateTag);
	
};
