// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PawnBasePlayer/SuraPawnPlayerMovementComponent.h"

#include "Components/CapsuleComponent.h"

USuraPawnPlayerMovementComponent::USuraPawnPlayerMovementComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = TG_PrePhysics;

	PrimaryState = EPrimaryState::Airborne;
	GroundedState = EGroundedState::Idle;
	AirborneState = EAirborneState::Falling;
	CrouchState = ECrouchState::NotCrouching;

	MovementInput = FVector2D::ZeroVector;
	
	Velocity = FVector::ZeroVector;
	GravityDirection = FVector::DownVector;
	Gravity = GravityDirection * GravityScale;

}


void USuraPawnPlayerMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerPawn = Cast<APawn>(GetOwner());
	if (PlayerPawn)
	{
		CapsuleComponent = PlayerPawn->FindComponentByClass<UCapsuleComponent>();
	}
	
	
}


void USuraPawnPlayerMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	

	if (MovementInput != FVector2D::ZeroVector)
	{
		FVector MovementInputDirection =
			(MovementInput.X * PlayerPawn->GetActorRightVector() + MovementInput.Y * PlayerPawn->GetActorForwardVector()).GetSafeNormal();
		Velocity += MovementInputDirection * Acceleration * DeltaTime;
		if (Velocity.SizeSquared() > WalkSpeed * WalkSpeed)
		{
			Velocity = Velocity.GetSafeNormal() * WalkSpeed;
		}
	}
	else
	{
		if (!Velocity.IsNearlyZero())
		{
			FVector DecelerationDirection = -Velocity.GetSafeNormal();
			float CurrentSpeed = Velocity.Size();
			float DeltaVelocity = Deceleration * DeltaTime;

			if (DeltaVelocity >= CurrentSpeed)
			{
				Velocity = FVector::ZeroVector;
			}
			else
			{
				Velocity += DecelerationDirection * DeltaVelocity;
			}
		}
		
	}

	if (Velocity != FVector::ZeroVector)
	{
		FVector CurrentLocation = PlayerPawn->GetActorLocation();
		FVector NewLocation = CurrentLocation + Velocity * DeltaTime;
		PlayerPawn->SetActorLocation(NewLocation, true);
	}
	

}

bool USuraPawnPlayerMovementComponent::CheckIsGrounded()
{
	if (!PlayerPawn || !CapsuleComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerPawn or CapsuleComponent not found!"));
	}

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(PlayerPawn);
	FCollisionShape GroundCheckSphere = FCollisionShape::MakeSphere(20.f);

	FVector TraceStart = PlayerPawn->GetActorLocation();
	FVector TraceEnd = TraceStart - FVector::DownVector * CapsuleComponent->GetScaledCapsuleHalfHeight();

	bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		TraceStart,
		TraceEnd,
		FQuat::Identity,
		ECC_Visibility,
		GroundCheckSphere,
		QueryParams
		);

	return bHit;
}

void USuraPawnPlayerMovementComponent::UpdateMovementState()
{
	if (CheckIsGrounded())
	{
		if (PrimaryState != EPrimaryState::Grounded)
		{
			PrimaryState = EPrimaryState::Grounded;
		}
	}
	else
	{
		if (PrimaryState != EPrimaryState::Airborne)
		{
			PrimaryState = EPrimaryState::Airborne;
		}
	}
}


void USuraPawnPlayerMovementComponent::ProcessMovement(float DeltaTime)
{
	switch (PrimaryState)
	{
		case EPrimaryState::Grounded:
			HandleGroundedMovement(DeltaTime);
			break;
		case EPrimaryState::Airborne:
			HandleAirborneMovement(DeltaTime);
			break;
		default:
			break;
	}
}

void USuraPawnPlayerMovementComponent::HandleGroundedMovement(float DeltaTime)
{
	Velocity.Z = 0.f;
	
	switch (GroundedState)
	{
		case EGroundedState::Idle:
			break;
		case EGroundedState::Walking:
			break;
		case EGroundedState::Sprinting:
			break;
		case EGroundedState::Crouching:
			break;
		case EGroundedState::Sliding:
			break;
		default:
			break;
	}
}

void USuraPawnPlayerMovementComponent::HandleAirborneMovement(float DeltaTime)
{
	
}



void USuraPawnPlayerMovementComponent::SetMovementInput(const FVector2D& InputVector)
{
	if (!PlayerPawn) return;
	
	MovementInput = InputVector;
}



