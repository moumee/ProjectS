// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PawnBasePlayer/SuraPlayerMovementComponent.h"

#include "AnimNodes/AnimNode_RandomPlayer.h"
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

	

	if (!PawnOwner || !UpdatedComponent)
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



void USuraPlayerMovementComponent::TickAirborne(float DeltaTime)
{
	if (PreviousMovementState == EMovementState::EMS_Move ||
		PreviousMovementState == EMovementState::EMS_WallRun)
	{
		ElapsedTimeFromSurface += DeltaTime;
	}
	
	if (IsGrounded())
	{
		if (PreviousMovementState != EMovementState::EMS_Move)
		{
			SetMovementState(EMovementState::EMS_Move);
			return;
		}
		else
		{
			if (ElapsedTimeFromSurface > JumpBuffer)
			{
				SetMovementState(EMovementState::EMS_Move);
				return;
			}
		}

		
	}

	if (CanWallRun())
	{
		if (PreviousMovementState != EMovementState::EMS_WallRun)
		{
			SetMovementState(EMovementState::EMS_WallRun);
			return;
		}
		else
		{
			if (ElapsedTimeFromSurface > WallJumpBuffer)
			{
				SetMovementState(EMovementState::EMS_WallRun);
				return;
			}
		}
	}

	
	
	const FVector InputDirection = ConsumeInputVector().GetSafeNormal();

	if (!bIsDashing)
	{
		float MaxHorizontalSpeed = RunSpeed;
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
		bJumpPressed = false;
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
		Velocity = DashDirection.GetSafeNormal2D() * DashStartSpeed + Velocity.Z;
	}
	
}

void USuraPlayerMovementComponent::TickWallRun(float DeltaTime)
{
	if (WallRunElapsedTime < WallRunMaxDuration)
	{
		WallRunElapsedTime += DeltaTime;
	}
	else
	{
		WallRunElapsedTime = 0.f;
		Velocity += CurrentWallHit.ImpactNormal * 100.f;
		SetMovementState(EMovementState::EMS_Airborne);
		return;
	}

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(SuraPawnPlayer);
	FVector TraceStart = SuraPawnPlayer->GetActorLocation();
	FVector TraceEnd = TraceStart - CurrentWallHit.ImpactNormal * 200.f;

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, Params);
	
	if (!bHit || !HitResult.IsValidBlockingHit())
	{
	    SetMovementState(EMovementState::EMS_Airborne);
	    return;
	}
	

	// Wall was hit - continue with wall run logic
	CurrentWallHit = HitResult;
	FVector VelocityDir;

	// Determine velocity direction based on which side we're running on
	if (CurrentWallRunSide == EWallRunSide::EWRS_Left)
	{
	    VelocityDir = FVector::CrossProduct(CurrentWallHit.ImpactNormal, FVector::UpVector).GetSafeNormal();
	    
	    // Check if player wants to exit wall to the right
	    if (MovementInputVector.X > 0.f)
	    {
	    	Velocity += CurrentWallHit.ImpactNormal * 100.f;
	        SetMovementState(EMovementState::EMS_Airborne);
	        return;
	    }
	}
	else // EWRS_Right
	{
	    VelocityDir = FVector::CrossProduct(CurrentWallHit.ImpactNormal, FVector::DownVector).GetSafeNormal();
	    
	    // Check if player wants to exit wall to the left
	    if (MovementInputVector.X < 0.f)
	    {
	    	Velocity += CurrentWallHit.ImpactNormal * 100.f;
	        SetMovementState(EMovementState::EMS_Airborne);
	        return;
	    }
	}

	// Input W key (Forward)
	if (MovementInputVector.Y > 0.f)
	{
		// If wall running forward direction
		if (FVector::DotProduct(Velocity.GetSafeNormal(), VelocityDir) > 0.f)
		{
			float NewSpeed = FMath::Clamp(Velocity.Size() + WallRunAcceleration * DeltaTime, -WallRunBackwardMaxSpeed, WallRunMaxSpeed);
			Velocity = NewSpeed * VelocityDir;
		}
		// If wall running backward direction
		else
		{
			float NewSpeed = FMath::Clamp(Velocity.Size() - WallRunAcceleration * DeltaTime, -WallRunMaxSpeed, WallRunBackwardMaxSpeed);
			Velocity = NewSpeed * -VelocityDir;
		}
	}
	// Input S Key (Backward)
	else if (MovementInputVector.Y < 0.f)
	{
		// If wall running forward direction
	    if (FVector::DotProduct(Velocity.GetSafeNormal(), VelocityDir) > 0.f)
	    {
	        float NewSpeed = FMath::Clamp(Velocity.Size() - WallRunDeceleration * DeltaTime, -WallRunBackwardMaxSpeed, WallRunMaxSpeed);
	        Velocity = NewSpeed * VelocityDir;
	    }
		// If wall running backward direction
	    else
	    {
	        float NewSpeed = FMath::Clamp(Velocity.Size() + WallRunDeceleration * DeltaTime, -WallRunMaxSpeed, WallRunBackwardMaxSpeed);
	        Velocity = NewSpeed * -VelocityDir;
	    }
	}
	else
	{
	    float NewSpeed = FMath::Max(Velocity.Size() - WallRunDeceleration * DeltaTime, 0.f);
	    Velocity = VelocityDir * NewSpeed;
	}

	if (bJumpPressed && CurrentJumpCount < MaxJumpCount)
	{
		bJumpPressed = false;
		CurrentJumpCount++;
		FVector WallNormal2D = CurrentWallHit.ImpactNormal.GetSafeNormal2D();
		Velocity += WallNormal2D * 500.f + FVector::UpVector * JumpZVelocity;
		SetMovementState(EMovementState::EMS_Airborne);
		return;
	}

	if (bCrouchPressed)
	{
		Velocity += CurrentWallHit.ImpactNormal * 100.f;
		SetMovementState(EMovementState::EMS_Airborne);
		return;
	}
	
}

void USuraPlayerMovementComponent::TickHang(float DeltaTime){}

void USuraPlayerMovementComponent::TickMantle(float DeltaTime){}

bool USuraPlayerMovementComponent::CanWallRun()
{
	if (bCrouchPressed) return false;
	

	FCollisionQueryParams WallQueryParams;
	WallQueryParams.AddIgnoredActor(SuraPawnPlayer);
	
	FVector WallTraceStart = PawnOwner->GetActorLocation();
	FVector WallTraceRightEnd = WallTraceStart + PawnOwner->GetActorRightVector() * 100.f;
	FVector WallTraceLeftEnd = WallTraceStart - PawnOwner->GetActorRightVector() * 100.f;

	FHitResult WallRightHit;
	bool bWallRightHit = GetWorld()->LineTraceSingleByChannel(WallRightHit, WallTraceStart, WallTraceRightEnd,
															  ECC_GameTraceChannel2, WallQueryParams);

	bool bRightWallRunnable = false;

	if (bWallRightHit && WallRightHit.IsValidBlockingHit() && WallRightHit.ImpactNormal.Z < MinWalkableFloorZ &&
		FVector::DotProduct(Velocity.GetSafeNormal2D(), WallRightHit.ImpactNormal.GetSafeNormal2D()) < 0.f)
	{
		bRightWallRunnable = true;
	}

	FHitResult WallLeftHit;
	bool bWallLeftHit = GetWorld()->LineTraceSingleByChannel(WallLeftHit, WallTraceStart, WallTraceLeftEnd,
															 ECC_WorldStatic, WallQueryParams);

	bool bLeftWallRunnable = false;

	if (bWallLeftHit && WallLeftHit.IsValidBlockingHit() && WallLeftHit.ImpactNormal.Z < MinWalkableFloorZ &&
		FVector::DotProduct(Velocity.GetSafeNormal2D(), WallLeftHit.ImpactNormal.GetSafeNormal2D()) < 0.f)
	{
		bLeftWallRunnable = true;
	}

	if (bRightWallRunnable && bLeftWallRunnable)
	{
		WallRunEnterSpeed2D = Velocity.Size2D();
		CurrentWallRunSide = WallLeftHit.Distance > WallRightHit.Distance ? EWallRunSide::EWRS_Right : EWallRunSide::EWRS_Left;
		CurrentWallHit = WallLeftHit.Distance > WallRightHit.Distance ? WallRightHit : WallLeftHit;
		return true;
	}
	else if (bLeftWallRunnable)
	{
		WallRunEnterSpeed2D = Velocity.Size2D();
		CurrentWallRunSide = EWallRunSide::EWRS_Left;
		CurrentWallHit = WallLeftHit;
		return true;
	}
	else if (bRightWallRunnable)
	{
		WallRunEnterSpeed2D = Velocity.Size2D();
		CurrentWallRunSide = EWallRunSide::EWRS_Right;
		CurrentWallHit = WallRightHit;
		return true;
	}
	else
	{
		CurrentWallRunSide = EWallRunSide::EWRS_None;
	}
	return false;
}



void USuraPlayerMovementComponent::OnMovementStateChanged(EMovementState OldState, EMovementState NewState)
{
	if (OldState == EMovementState::EMS_Airborne)
	{
		ElapsedTimeFromSurface = 0.f;
		CurrentJumpCount = 0;
	}

	if (NewState == EMovementState::EMS_WallRun)
	{
		Velocity = Velocity.GetSafeNormal2D() * WallRunEnterSpeed2D;
	}

	if (OldState == EMovementState::EMS_WallRun)
	{
		WallRunElapsedTime = 0.f;
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




