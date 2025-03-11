// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SuraPawnPlayerStates.h"
#include "Components/ActorComponent.h"
#include "SuraPawnPlayerMovementComponent.generated.h"


class UCapsuleComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SURAS_API USuraPawnPlayerMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USuraPawnPlayerMovementComponent();
	
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetMovementInput(const FVector2D& InputVector);

protected:

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	EPrimaryState PrimaryState;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	EGroundedState GroundedState;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	EAirborneState AirborneState;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	ECrouchState CrouchState;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float GravityScale = 980.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float WalkSpeed = 1000.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float SprintSpeed = 2000.f;
	
	UPROPERTY(EditAnywhere, Category = "Movement")
	float Acceleration = 4000.f;

	UPROPERTY(EditAnywhere, Category = "Movement")
	float Deceleration = 2000.f;

	UPROPERTY()
	APawn* PlayerPawn = nullptr;

	UPROPERTY()
	UCapsuleComponent* CapsuleComponent = nullptr;

	FVector2D MovementInput;

	FVector Velocity;
	
	FVector Gravity;
	FVector GravityDirection;

	bool CheckIsGrounded();
	
	void ProcessMovement(float DeltaTime);

	void UpdateMovementState();

	void HandleGroundedMovement(float DeltaTime);
	void HandleAirborneMovement(float DeltaTime);

		
};





