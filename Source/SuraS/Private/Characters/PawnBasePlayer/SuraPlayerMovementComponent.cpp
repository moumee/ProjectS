// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PawnBasePlayer/SuraPlayerMovementComponent.h"

#include "AITypes.h"
#include "Characters/PawnBasePlayer/SuraPawnPlayer.h"
#include "Components/CapsuleComponent.h"

USuraPlayerMovementComponent::USuraPlayerMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = TG_PrePhysics;

	MinWalkableFloorZ = FMath::Cos(FMath::DegreesToRadians(MaxWalkableFloorAngle));
}

void USuraPlayerMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (PawnOwner)
	{
		SuraPawnPlayer = Cast<ASuraPawnPlayer>(PawnOwner);
	}

	GravityAcceleration = GravityDirection * GravityScale;
	CurrentMovementState = EMovementState::EMS_Airborne;

}



void USuraPlayerMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
                                                 FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	GEngine->AddOnScreenDebugMessage(
		0, 0.f, FColor::Cyan, FString::Printf(TEXT("Current State : %s"), *UEnum::GetValueAsString(CurrentMovementState)));

	GEngine->AddOnScreenDebugMessage(
		1, 0.f, FColor::Cyan, FString::Printf(TEXT("Velocity : (%f, %f, %f) / Size2D : %f "), Velocity.X, Velocity.Y, Velocity.Z, Velocity.Size2D()));

	if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime))
	{
		return;
	}

	TickState(DeltaTime);

	FVector DesiredTickMovement = Velocity * DeltaTime;
	if (!DesiredTickMovement.IsNearlyZero())
	{
		FHitResult Hit;
		SafeMoveUpdatedComponent(DesiredTickMovement, UpdatedComponent->GetComponentRotation(), true, Hit);
		if (Hit.IsValidBlockingHit())
		{
			SlideAlongSurface(DesiredTickMovement, 1.f - Hit.Time, Hit.Normal, Hit);
		}
	}
	

	
}

void USuraPlayerMovementComponent::TickState(float DeltaTime)
{
	switch (CurrentMovementState)
	{
		case EMovementState::EMS_Move:
			TickMove(DeltaTime);
			break;
		case EMovementState::EMS_Slide:
			TickSlide(DeltaTime);
			break;
		case EMovementState::EMS_Airborne:
			TickAirborne(DeltaTime);
			break;
		case EMovementState::EMS_Dash:
			TickDash(DeltaTime);
			break;
		case EMovementState::EMS_WallRun:
			TickWallRun(DeltaTime);
			break;
		case EMovementState::EMS_Hang:
			TickHang(DeltaTime);
			break;
		case EMovementState::EMS_Mantle:
			TickMantle(DeltaTime);
			break;
		default:
			UE_LOG(LogTemp, Error, TEXT("CurrentMovementState not valid"));
			break;
	}
}

void USuraPlayerMovementComponent::TickMove(float DeltaTime)
{

	if (!IsGrounded())
	{
		SetMovementState(EMovementState::EMS_Airborne);
		return;
	}
	
	const FVector InputDirection = ConsumeInputVector().GetSafeNormal();

	if (MovementInputVector.IsNearlyZero())
	{
		if (Velocity.Size() > 0.f)
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
	}
	else
	{
		float WishSpeed = MovementInputVector.Y > 0.f ? RunSpeed : WalkSpeed;
		Velocity = (Velocity + InputDirection * Acceleration * DeltaTime).GetClampedToMaxSize2D(WishSpeed);
	}

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(SuraPawnPlayer);

	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, SuraPawnPlayer->GetActorLocation(),
		SuraPawnPlayer->GetActorLocation() - FVector(0, 0, SuraPawnPlayer->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 10.f),
		ECC_WorldStatic, Params);

	if (bHit && Hit.IsValidBlockingHit())
	{
		if (Hit.ImpactNormal.Z >= MinWalkableFloorZ)
		{
			Velocity = FVector::VectorPlaneProject(Velocity, Hit.ImpactNormal).GetSafeNormal() * Velocity.Size();
		}
	}

	if (bJumpPressed)
	{
		Velocity.Z = FMath::Sqrt(2 * GravityScale * JumpHeight);
		SetMovementState(EMovementState::EMS_Airborne);
		return;
	}
	
}

void USuraPlayerMovementComponent::TickSlide(float DeltaTime)
{
	
}

void USuraPlayerMovementComponent::TickAirborne(float DeltaTime)
{
	if (IsGrounded())
	{
		SetMovementState(EMovementState::EMS_Move);
		return;
	}
	
	const FVector InputDirection = ConsumeInputVector().GetSafeNormal();


	Velocity = (Velocity + InputDirection * AirAcceleration * DeltaTime).GetClampedToMaxSize2D(RunSpeed);

	Velocity += GravityAcceleration * DeltaTime;
	if (Velocity.Z < -MaxFallVerticalSpeed)
	{
		Velocity.Z *= MaxFallVerticalSpeed / FMath::Abs(Velocity.Z);
	}
	
}

void USuraPlayerMovementComponent::TickDash(float DeltaTime){}

void USuraPlayerMovementComponent::TickWallRun(float DeltaTime){}

void USuraPlayerMovementComponent::TickHang(float DeltaTime){}

void USuraPlayerMovementComponent::TickMantle(float DeltaTime){}

void USuraPlayerMovementComponent::OnMovementStateChanged(EMovementState NewState)
{
	
}



bool USuraPlayerMovementComponent::IsGrounded() const
{
	FHitResult GroundSweepHit;
	FCollisionQueryParams GroundSweepParams;
	GroundSweepParams.AddIgnoredActor(PawnOwner);

	FVector SweepStart = SuraPawnPlayer->GetActorLocation();
	FVector SweepEnd = SuraPawnPlayer->GetActorLocation() +
		FVector::DownVector * (SuraPawnPlayer->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 10.f);
	
	bool bHit = GetWorld()->SweepSingleByChannel(GroundSweepHit, SweepStart, SweepEnd, FQuat::Identity, ECC_WorldStatic,
		FCollisionShape::MakeSphere(40.f), GroundSweepParams);
	

	if (!bHit || !GroundSweepHit.bBlockingHit)
	{
		return false;
	}

	// if (GroundSweepHit.ImpactNormal.Z < MinWalkableFloorZ)
	// {
	// 	return false;
	// }

	return true;
}



void USuraPlayerMovementComponent::SetMovementInputVector(const FVector2D& InMovementInputVector)
{
	MovementInputVector = InMovementInputVector;
}

void USuraPlayerMovementComponent::SetMovementState(EMovementState NewState)
{
	PreviousMovementState = CurrentMovementState;
	CurrentMovementState = NewState;

	OnMovementStateChanged(NewState);
}

void USuraPlayerMovementComponent::SetJumpPressed(bool bPressed)
{
	bJumpPressed = bPressed;
}


void USuraPlayerMovementComponent::SetDashPressed(bool bPressed)
{
	bDashPressed = bPressed;
}


void USuraPlayerMovementComponent::SetCrouchPressed(bool bPressed)
{
	bCrouchPressed = bPressed;
}



