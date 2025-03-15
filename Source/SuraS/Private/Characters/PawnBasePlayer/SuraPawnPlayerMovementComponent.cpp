// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PawnBasePlayer/SuraPawnPlayerMovementComponent.h"

#include "Components/CapsuleComponent.h"

USuraPawnPlayerMovementComponent::USuraPawnPlayerMovementComponent()
{
	
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = TG_PrePhysics;

	PrimaryState = EPrimaryState::Grounded;
	GroundedState = EGroundedState::Idle;
	AirborneState = EAirborneState::None;
	CrouchState = ECrouchState::NotCrouching;

	MovementInput = FVector2D::ZeroVector;
	MovementInputDirection = FVector::ZeroVector;
	
	Velocity = FVector::ZeroVector;
	GravityDirection = FVector::DownVector;
	Gravity = GravityDirection * GravityScale;

}

void USuraPawnPlayerMovementComponent::ShowDebugInfo()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(0, 0.f, FColor::Blue,
			FString::Printf(TEXT("%s %s %s %s"),
				*UEnum::GetValueAsString(PrimaryState), *UEnum::GetValueAsString(GroundedState),
				*UEnum::GetValueAsString(AirborneState), *UEnum::GetValueAsString(CrouchState)));

		GEngine->AddOnScreenDebugMessage(1, 0.f, FColor::Green,
			FString::Printf(TEXT("Velocity Magnitude : %f"), Velocity.Size()));
	}
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

	ShowDebugInfo();

	HandleState(DeltaTime);

	ApplyVelocity(DeltaTime);

}

void USuraPawnPlayerMovementComponent::CheckIsGrounded()
{
	if (!PlayerPawn || !CapsuleComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerPawn or CapsuleComponent not found!"));
	}

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(PlayerPawn);
	FCollisionShape GroundCheckSphere = FCollisionShape::MakeSphere(40.f);

	FVector TraceStart = PlayerPawn->GetActorLocation();
	FVector TraceEnd = TraceStart + FVector::DownVector * CapsuleComponent->GetScaledCapsuleHalfHeight();

	bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		TraceStart,
		TraceEnd,
		FQuat::Identity,
		ECC_Visibility,
		GroundCheckSphere,
		QueryParams
		);

	bIsGrounded = bHit;
}


void USuraPawnPlayerMovementComponent::HandleState(float DeltaTime)
{
	if (MovementInput != FVector2D::ZeroVector)
	{
		MovementInputDirection =
			(MovementInput.X * PlayerPawn->GetActorRightVector() + MovementInput.Y * PlayerPawn->GetActorForwardVector()).GetSafeNormal();
	}
	else
	{
		MovementInputDirection = FVector::ZeroVector;
	}

	CheckIsGrounded();
	
	switch (PrimaryState)
	{
		case EPrimaryState::Grounded:
			HandleGrounded(DeltaTime);
			break;
		case EPrimaryState::Airborne:
			HandleAirborne(DeltaTime);
			break;
		case EPrimaryState::WallRunning:
			HandleWallRunning(DeltaTime);
			break;
		case EPrimaryState::Mantling:
			HandleMantling(DeltaTime);
			break;
		case EPrimaryState::Hanging:
			HandleHanging(DeltaTime);
		default:
			break;
	}
}

void USuraPawnPlayerMovementComponent::HandleGrounded(float DeltaTime)
{
	if (!bIsGrounded)
	{
		PrimaryState = EPrimaryState::Airborne;
		AirborneState = EAirborneState::Falling;
		GroundedState = EGroundedState::None;
		return;
	}

	// if (bDashPressed) {}
	
	switch (GroundedState)
	{
		case EGroundedState::Idle:
			HandleIdle(DeltaTime);
			break;
		case EGroundedState::Walking:
			HandleWalking(DeltaTime);
			break;
		case EGroundedState::Sprinting:
			HandleSprinting(DeltaTime);
			break;
		case EGroundedState::Crouching:
			HandleCrouching(DeltaTime);
			break;
		case EGroundedState::Sliding:
			HandleSliding(DeltaTime);
			break;
		default:
			break;
	}
	

	
}

void USuraPawnPlayerMovementComponent::HandleIdle(float DeltaTime)
{
	if (MovementInputDirection != FVector::ZeroVector)
	{
		if (MovementInput.Y > 0.f)
		{
			GroundedState = EGroundedState::Sprinting;
			return;
		}
		else
		{
			GroundedState = EGroundedState::Walking;
			return;
		}
	}
	else
	{
		FVector DeltaVelocity = -Velocity.GetSafeNormal() * Deceleration * DeltaTime;
		if (DeltaVelocity.Size() > Velocity.Size())
		{
			Velocity = FVector::ZeroVector;
		}
		else
		{
			Velocity += DeltaVelocity;
		}
	}

	if (bJumpPressed)
	{
		if (MovementInput != FVector2D::ZeroVector)
		{
			Velocity += MovementInputDirection * Acceleration * DeltaTime;
			
		} 
		Velocity.Z = JumpForce;
		PrimaryState = EPrimaryState::Airborne;
		AirborneState = EAirborneState::Jumping;
		GroundedState = EGroundedState::None;
		return;
	}
}

void USuraPawnPlayerMovementComponent::HandleWalking(float DeltaTime)
{
	if (MovementInputDirection != FVector::ZeroVector)
	{
		Velocity += MovementInputDirection * Acceleration * DeltaTime;
		if (Velocity.Size() > WalkSpeed)
		{
			Velocity = Velocity.GetSafeNormal() * WalkSpeed;
		}

		if (MovementInput.Y > 0.f)
		{
			GroundedState = EGroundedState::Sprinting;
			return;
		}
	}
	else
	{
		GroundedState = EGroundedState::Idle;
	}
	
	if (bJumpPressed)
	{
		Velocity.Z = JumpForce;
		PrimaryState = EPrimaryState::Airborne;
		AirborneState = EAirborneState::Jumping;
		GroundedState = EGroundedState::None;
		return;
	}
}

void USuraPawnPlayerMovementComponent::HandleSprinting(float DeltaTime)
{
	if (MovementInputDirection != FVector::ZeroVector)
	{
		Velocity += MovementInputDirection * Acceleration * DeltaTime;
		if (Velocity.Size() > SprintSpeed)
		{
			Velocity = Velocity.GetSafeNormal() * SprintSpeed;
		}

		if (MovementInput.Y <= 0.f)
		{
			GroundedState = EGroundedState::Walking;
			return;
		}
	}
	else
	{
		GroundedState = EGroundedState::Idle;
		return;
	}
	
	if (bJumpPressed)
	{
		Velocity.Z = JumpForce;
		PrimaryState = EPrimaryState::Airborne;
		AirborneState = EAirborneState::Jumping;
		GroundedState = EGroundedState::None;
		return;
	}
}

void USuraPawnPlayerMovementComponent::HandleCrouching(float DeltaTime)
{
}

void USuraPawnPlayerMovementComponent::HandleSliding(float DeltaTime)
{
}




void USuraPawnPlayerMovementComponent::HandleAirborne(float DeltaTime)
{
	Velocity += Gravity * DeltaTime;

	if (MovementInput != FVector2D::ZeroVector)
	{
		Velocity += MovementInputDirection * Acceleration * DeltaTime;
		FVector HorizontalVelocity = FVector(Velocity.X, Velocity.Y, 0.f);
		if (HorizontalVelocity.Size() > 1000.f)
		{
			Velocity = HorizontalVelocity.GetSafeNormal() * 1000.f + FVector(0.f, 0.f, Velocity.Z);
		}
		
	} 

	if (bIsGrounded && Velocity.Z <= 0.f)
	{
		Velocity.Z = 0.f;
		PrimaryState = EPrimaryState::Grounded;
		GroundedState = EGroundedState::Idle;
	}
	
	switch (AirborneState)
	{
		case EAirborneState::Falling:
			break;
		case EAirborneState::Jumping:
			break;
		case EAirborneState::DoubleJumping:
			break;
		default:
			break;
	}
}

void USuraPawnPlayerMovementComponent::HandleFalling(float DeltaTime)
{
	
}

void USuraPawnPlayerMovementComponent::HandleJumping(float DeltaTime)
{
}

void USuraPawnPlayerMovementComponent::HandleDoubleJumping(float DeltaTime)
{
}

void USuraPawnPlayerMovementComponent::HandleWallRunning(float DeltaTime)
{
}

void USuraPawnPlayerMovementComponent::HandleMantling(float DeltaTime)
{
}

void USuraPawnPlayerMovementComponent::HandleHanging(float DeltaTime)
{
}

void USuraPawnPlayerMovementComponent::ApplyVelocity(float DeltaTime)
{
	if (Velocity != FVector::ZeroVector)
	{
		FVector CurrentLocation = PlayerPawn->GetActorLocation();
		FVector DeltaVelocity = Velocity * DeltaTime;

		FVector ComputedLocation = CollideAndSlide(CurrentLocation, DeltaVelocity, 0);
		
		PlayerPawn->SetActorLocation(ComputedLocation, false);
	}
}

FVector USuraPawnPlayerMovementComponent::CollideAndSlide(const FVector& StartPos, const FVector& DeltaVelocity, int32 Depth)
{
	if (Depth >= 3)
	{
		return StartPos; 
	}

	FVector TargetPos = StartPos + DeltaVelocity;

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(PlayerPawn);
	bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		StartPos,
		TargetPos,
		FQuat::Identity,
		ECC_Visibility,
		FCollisionShape::MakeCapsule(CapsuleComponent->GetScaledCapsuleRadius(), CapsuleComponent->GetScaledCapsuleHalfHeight()),
		QueryParams);

	if (bHit)
	{
		FVector RemainingDeltaVelocity = TargetPos - HitResult.Location;
		FVector NewDeltaVelocity =
			FVector::VectorPlaneProject(RemainingDeltaVelocity, HitResult.ImpactNormal).GetSafeNormal() * RemainingDeltaVelocity.Size() * 0.8f;
		return CollideAndSlide(HitResult.Location + HitResult.ImpactNormal * 0.1f, NewDeltaVelocity, Depth + 1);
	}

	return TargetPos;
}



void USuraPawnPlayerMovementComponent::SetMoveInput(const FVector2D& InputVector)
{
	MovementInput = InputVector;
}

void USuraPawnPlayerMovementComponent::SetJumpInput(bool bPressed)
{
	bJumpPressed = bPressed;
}

void USuraPawnPlayerMovementComponent::SetDashInput(bool bPressed)
{
	bDashPressed = bPressed;
}

void USuraPawnPlayerMovementComponent::SetCrouchInput(bool bPressed)
{
	bCrouchPressed = bPressed;
}





