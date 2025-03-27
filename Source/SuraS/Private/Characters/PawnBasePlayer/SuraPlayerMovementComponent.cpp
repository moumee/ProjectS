// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PawnBasePlayer/SuraPlayerMovementComponent.h"

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
			FVector CalculatedVelocity = Velocity.GetSafeNormal() * FMath::InterpEaseIn(DashStartSpeed, DashEndSpeed, T, 2.f);
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

bool USuraPlayerMovementComponent::TryWallRun()
{
	FCollisionQueryParams WallQueryParams;
	WallQueryParams.AddIgnoredActor(SuraPawnPlayer);
	
	FVector WallTraceStart = PawnOwner->GetActorLocation();
	FVector WallTraceRightEnd = WallTraceStart + PawnOwner->GetActorRightVector() * 200.f;
	FVector WallTraceLeftEnd = WallTraceStart - PawnOwner->GetActorRightVector() * 200.f;

	FHitResult WallRightHit;
	bool bWallRightHit = GetWorld()->LineTraceSingleByChannel(WallRightHit, WallTraceStart, WallTraceRightEnd,
	                                                          ECC_WorldStatic, WallQueryParams);

	bool bRightWallRunnable = false;

	if (bWallRightHit && WallRightHit.IsValidBlockingHit() && WallRightHit.ImpactNormal.Z < MinWalkableFloorZ)
	{
		bRightWallRunnable = true;
	}

	FHitResult WallLeftHit;
	bool bWallLeftHit = GetWorld()->LineTraceSingleByChannel(WallLeftHit, WallTraceStart, WallTraceLeftEnd,
	                                                         ECC_WorldStatic, WallQueryParams);

	bool bLeftWallRunnable = false;

	if (bWallLeftHit && WallLeftHit.IsValidBlockingHit() && WallLeftHit.ImpactNormal.Z < MinWalkableFloorZ)
	{
		bLeftWallRunnable = true;
	}

	if (bRightWallRunnable && bLeftWallRunnable)
	{
		CurrentWallSide = WallLeftHit.Distance > WallRightHit.Distance ? EWallRunSide::EWRS_Right : EWallRunSide::EWRS_Left;
		CurrentWallHit = WallLeftHit.Distance > WallRightHit.Distance ? WallRightHit : WallLeftHit;
		SetMovementState(EMovementState::EMS_WallRun);
		return true;
	}
	else if (bLeftWallRunnable)
	{
		CurrentWallSide = EWallRunSide::EWRS_Left;
		CurrentWallHit = WallLeftHit;
		SetMovementState(EMovementState::EMS_WallRun);
		return true;
	}
	else if (bRightWallRunnable)
	{
		CurrentWallSide = EWallRunSide::EWRS_Right;
		CurrentWallHit = WallRightHit;
		SetMovementState(EMovementState::EMS_WallRun);
		return true;
	}
	else
	{
		CurrentWallSide = EWallRunSide::EWRS_None;
	}
	return false;
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

	if (TryWallRun()) return;
	
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

void USuraPlayerMovementComponent::TickWallRun(float DeltaTime)
{
	float EnterZSpeed = Velocity.Z;

	// Implement later
	if (EnterZSpeed >= 0.f && EnterZSpeed < 30.f)
	{
		
	}
	else if (EnterZSpeed < 0.f && EnterZSpeed > -30.f)
	{
		
	}
	else
	{
		
	}

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(SuraPawnPlayer);
	FVector TraceStart = SuraPawnPlayer->GetActorLocation();
	FVector TraceEnd = TraceStart - CurrentWallHit.ImpactNormal * 200.f;

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility,
		Params);
	
	if (bHit && HitResult.IsValidBlockingHit())
	{
		CurrentWallHit = HitResult;
		if (CurrentWallSide == EWallRunSide::EWRS_Left)
		{
			FVector VelocityDir = FVector::CrossProduct(CurrentWallHit.ImpactNormal, FVector::UpVector).GetSafeNormal();
			Velocity = VelocityDir * 1000.f;
		}
		else if (CurrentWallSide == EWallRunSide::EWRS_Right)
		{
			FVector VelocityDir = FVector::CrossProduct(CurrentWallHit.ImpactNormal, FVector::DownVector).GetSafeNormal();
			Velocity = VelocityDir * 1000.f;
		}
		
	}
	
	
}

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
		FVector::DownVector * (SuraPawnPlayer->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
	
	bool bHit = GetWorld()->SweepSingleByChannel(GroundSweepHit, SweepStart, SweepEnd, FQuat::Identity, ECC_WorldStatic,
		FCollisionShape::MakeSphere(20.f), GroundSweepParams);
	

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

bool USuraPlayerMovementComponent::TryStepUp(const FHitResult& Hit, const FVector& DeltaMove, float DeltaTime)
{
	if (!SuraPawnPlayer || MaxStepHeight <= 0.f)
	{
		return false;
	}

	const UCapsuleComponent* Capsule = SuraPawnPlayer->GetCapsuleComponent();

	const FVector CurrentLocation = UpdatedComponent->GetComponentLocation();
	const float CapsuleHalfHeight = Capsule->GetScaledCapsuleHalfHeight();
	const float CapsuleRadius = Capsule->GetScaledCapsuleRadius();
	const FVector HorizontalMoveDirection = DeltaMove.GetSafeNormal2D();

	// --- 1. Initial Downward Check from slightly above impact point ---
	// This confirms we hit something near our base, not high up the capsule
	FVector CheckStart = Hit.ImpactPoint + Hit.ImpactNormal * 0.1f; // Start slightly away from surface
	CheckStart.Z = CurrentLocation.Z; // Align Z with character center initially
	FVector CheckEnd = CheckStart - FVector(0, 0, CapsuleHalfHeight + MaxStepHeight + 5.0f); // Check down further than max step height

	FHitResult InitialDownHit;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(PawnOwner);

	if (!GetWorld()->LineTraceSingleByChannel(InitialDownHit, CheckStart, CheckEnd, ECC_Visibility, QueryParams))
	{
		// Didn't hit anything below impact point? Strange scenario, abort.
		return false;
	}

    // Calculate height difference from character base to the initial impact point's floor
	float InitialFloorZ = InitialDownHit.ImpactPoint.Z;
	float InitialStepHeight = InitialFloorZ - (CurrentLocation.Z - CapsuleHalfHeight);

    // If the floor below the impact point is already too high, it's not a step we can take
	if (InitialStepHeight > MaxStepHeight || InitialStepHeight <= KINDA_SMALL_NUMBER)
	{
		return false;
	}
     if (InitialDownHit.ImpactNormal.Z < MinWalkableFloorZ)
    {
         return false; // Initial surface below impact isn't walkable
    }


	// --- 2. Upward Check for Clearance ---
	// Check if there's space above the potential step ledge
	FVector UpCheckStart = InitialDownHit.ImpactPoint + FVector(0, 0, 0.1f); // Start just above the found floor
    FVector UpCheckEnd = UpCheckStart + FVector(0, 0, MaxStepHeight + CapsuleHalfHeight * 2.0f); // Check up high enough for capsule

	FCollisionShape CapsuleShape = FCollisionShape::MakeCapsule(CapsuleRadius, CapsuleHalfHeight);
	FHitResult UpHit;


	bool bHitUp = GetWorld()->SweepSingleByChannel(
		UpHit,
		UpCheckStart, // Start slightly above the potential step surface
		UpCheckStart, // End at same location for a check, not a move trace
		FQuat::Identity,
		ECC_Visibility, // Or your character collision channel
		CapsuleShape,
		QueryParams
	);

	// If the upward check hits something right away, there's no room to step up.
	if (bHitUp && UpHit.bBlockingHit)
	{
		return false;
	}


	// --- 3. Perform the Step Up ---
	// We have confirmed a step of appropriate height and clearance above it.
	const float TargetZ = InitialFloorZ + CapsuleHalfHeight + 1.0f; // Target center Z slightly above the step surface
	const FVector TargetLocation = FVector(CurrentLocation.X, CurrentLocation.Y, TargetZ); // Keep X/Y for now

	// Move the character vertically. Use TeleportPhysics to avoid physics interactions during the snap.
	UpdatedComponent->SetWorldLocation(TargetLocation, false, nullptr, ETeleportType::TeleportPhysics);

	// --- 4. Continue Horizontal Movement ---
	// Since we consumed the initial hit time, calculate remaining time and move horizontally
	float RemainingTime = DeltaTime * (1.f - Hit.Time);
    if (RemainingTime > KINDA_SMALL_NUMBER)
    {
        FVector RemainingHorizontalMove = Velocity.GetSafeNormal2D() * Velocity.Size2D() * RemainingTime;
        FHitResult MoveAfterStepHit;
        SafeMoveUpdatedComponent(RemainingHorizontalMove, UpdatedComponent->GetComponentRotation(), true, MoveAfterStepHit);

        // Optional: Handle sliding if we hit something *after* stepping up
        if (MoveAfterStepHit.IsValidBlockingHit())
        {
            SlideAlongSurface(RemainingHorizontalMove, 1.f - MoveAfterStepHit.Time, MoveAfterStepHit.Normal, MoveAfterStepHit);
        }
    }


	// Crucial: Reset velocity Z component after stepping up, otherwise gravity might be instantly reapplied too strongly
	Velocity.Z = FMath::Max(0.f, Velocity.Z); // Don't want downward velocity right after step up

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




