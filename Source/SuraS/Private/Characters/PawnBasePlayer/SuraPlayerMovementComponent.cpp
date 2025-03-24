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

	DashCooldowns.Init(0.f, 2);
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
	JumpZVelocity = FMath::Sqrt(2 * GravityScale * JumpHeight);

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

	UpdateDashCooldowns(DeltaTime);

	TickState(DeltaTime);

	FVector DesiredTickMovement = Velocity * DeltaTime;
	if (!DesiredTickMovement.IsNearlyZero())
	{
		FHitResult Hit;
		SafeMoveUpdatedComponent(DesiredTickMovement, UpdatedComponent->GetComponentRotation(), true, Hit);
		if (Hit.IsValidBlockingHit())
		{
			if (CurrentMovementState == EMovementState::EMS_Move && Hit.ImpactNormal.Z < MinWalkableFloorZ)
			{
				FVector AdjustedTickMovement = FVector(DesiredTickMovement.X, DesiredTickMovement.Y, 0).GetSafeNormal() * DesiredTickMovement.Size();
				FVector AdjustedNormal = FVector(Hit.Normal.X, Hit.Normal.Y, 0).GetSafeNormal();
				SlideAlongSurface(AdjustedTickMovement, 1.f - Hit.Time, AdjustedNormal, Hit);
			}
			else
			{
				SlideAlongSurface(DesiredTickMovement, 1.f - Hit.Time, Hit.Normal, Hit);
			}
			
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

	if (!bIsDashing)
	{
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
	}
	else
	{
		if (ElapsedTimeFromDash < DashDecelerationTime)
		{
			ElapsedTimeFromDash += DeltaTime;
			float T = ElapsedTimeFromDash / DashDecelerationTime;
			FVector CalculatedVelocity = Velocity.GetSafeNormal() * FMath::Lerp(DashStartSpeed, DashEndSpeed, T * T);
			Velocity = CalculatedVelocity;
		}
		else
		{
			bIsDashing = false;
			ElapsedTimeFromDash = 0.f;
			Velocity = Velocity.GetSafeNormal() * DashEndSpeed;
		}
	}

	

	FHitResult Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(SuraPawnPlayer);

	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, SuraPawnPlayer->GetActorLocation(),
		SuraPawnPlayer->GetActorLocation() - FVector(0, 0, SuraPawnPlayer->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 20.f),
		ECC_WorldStatic, Params);

	if (bHit && Hit.IsValidBlockingHit() && !bIsDashing)
	{
		if (Hit.ImpactNormal.Z >= MinWalkableFloorZ)
		{
			Velocity = FVector::VectorPlaneProject(Velocity, Hit.ImpactNormal).GetSafeNormal() * Velocity.Size();
		}
	}

	if (bJumpPressed && CurrentJumpCount < MaxJumpCount)
	{
		bJumpPressed = false;
		CurrentJumpCount++;
		Velocity.Z = JumpZVelocity;
		SetMovementState(EMovementState::EMS_Airborne);
		return;
	}

	if (bDashPressed && AvailableDashCount > 0)
	{
		bDashPressed = false;
		bIsDashing = true;
		
		for (int32 i = 0; i < DashCooldowns.Num(); i++)
		{
			if (DashCooldowns[i] == 0.f)
			{
				AvailableDashCount--;
				DashCooldowns[i] = DashCooldown;
				break;
			}
		}

		const FVector DashDirection = InputDirection.IsNearlyZero() ? PawnOwner->GetActorForwardVector() : InputDirection;
		Velocity = DashDirection * DashStartSpeed;
	}
	
}

void USuraPlayerMovementComponent::TickSlide(float DeltaTime)
{
	
}

void USuraPlayerMovementComponent::TickAirborne(float DeltaTime)
{
	if (PreviousMovementState == EMovementState::EMS_Move)
	{
		ElapsedTimeFromGround += DeltaTime;
	}
	
	if (IsGrounded())
	{
		if (ElapsedTimeFromGround > JumpBuffer)
		{
			SetMovementState(EMovementState::EMS_Move);
			return;
		}
	}
	
	const FVector InputDirection = ConsumeInputVector().GetSafeNormal();

	if (!bIsDashing)
	{
		float MaxHorizontalSpeed = RunSpeed * 0.8f;
		if (Velocity.Size2D() > MaxHorizontalSpeed)
		{
			Velocity = (Velocity - Velocity.GetSafeNormal() * AirDeceleration * DeltaTime).GetClampedToSize2D(MaxHorizontalSpeed, FLT_MAX);
		}
		else
		{
			Velocity = (Velocity + InputDirection * AirAcceleration * DeltaTime).GetClampedToMaxSize2D(MaxHorizontalSpeed);
		}
	}
	else
	{
		if (ElapsedTimeFromDash < DashDecelerationTime)
		{
			ElapsedTimeFromDash += DeltaTime;
			float T = ElapsedTimeFromDash / DashDecelerationTime;
			FVector HorizontalVelocity = Velocity.GetSafeNormal2D() * FMath::Lerp(DashStartSpeed, DashEndSpeed, T * T);
			Velocity = FVector(HorizontalVelocity.X, HorizontalVelocity.Y, Velocity.Z);
		}
		else
		{
			bIsDashing = false;
			ElapsedTimeFromDash = 0.f;
			FVector HorizontalVelocity = Velocity.GetSafeNormal2D() * DashEndSpeed;
			Velocity = FVector(HorizontalVelocity.X, HorizontalVelocity.Y, Velocity.Z);
		}
	}

	Velocity += GravityAcceleration * DeltaTime;
	if (Velocity.Z < -MaxFallVerticalSpeed)
	{
		Velocity.Z *= MaxFallVerticalSpeed / FMath::Abs(Velocity.Z);
	}

	if (bJumpPressed)
	{
		if (CurrentJumpCount < MaxJumpCount)
		{
			CurrentJumpCount++;
			Velocity.Z = JumpZVelocity;
		}
	}

	if (bDashPressed && AvailableDashCount > 0)
	{
		bDashPressed = false;
		bIsDashing = true;
		
		for (int32 i = 0; i < DashCooldowns.Num(); i++)
		{
			if (DashCooldowns[i] == 0.f)
			{
				AvailableDashCount--;
				DashCooldowns[i] = DashCooldown;
				break;
			}
		}

		const FVector DashDirection = InputDirection.IsNearlyZero() ? PawnOwner->GetActorForwardVector() : InputDirection;
		Velocity = DashDirection * DashStartSpeed;
	}
	
}

void USuraPlayerMovementComponent::TickWallRun(float DeltaTime){}

void USuraPlayerMovementComponent::TickHang(float DeltaTime){}

void USuraPlayerMovementComponent::TickMantle(float DeltaTime){}

void USuraPlayerMovementComponent::OnMovementStateChanged(EMovementState OldState, EMovementState NewState)
{
	if (OldState == EMovementState::EMS_Airborne)
	{
		CurrentJumpCount = 0;
		ElapsedTimeFromGround = 0.f;
	}
	
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

	if (GroundSweepHit.ImpactNormal.Z < MinWalkableFloorZ)
	{
		return false;
	}

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

	OnMovementStateChanged(PreviousMovementState, NewState);
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

void USuraPlayerMovementComponent::UpdateDashCooldowns(float DeltaTime)
{
	float MinCooldownLeftIndex = -1;
	
	if (AvailableDashCount < DashCooldowns.Num())
	{
		float MinDashCooldown = FLT_MAX;
		for (int32 i = 0; i < DashCooldowns.Num(); i++)
		{
			if (DashCooldowns[i] > 0 && DashCooldowns[i] < MinDashCooldown)
			{
				MinDashCooldown = DashCooldowns[i];
				MinCooldownLeftIndex = i;
			}
		}
	}

	if (MinCooldownLeftIndex > -1)
	{
		DashCooldowns[MinCooldownLeftIndex] = FMath::Clamp(DashCooldowns[MinCooldownLeftIndex] - DeltaTime, 0.f, DashCooldown);
		if (DashCooldowns[MinCooldownLeftIndex] <= 0.f)
		{
			AvailableDashCount++;
		}
	}
}




