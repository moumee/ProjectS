// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PawnBasePlayer/SuraPlayerMovementComponent.h"

#include "Camera/CameraComponent.h"
#include "Characters/PawnBasePlayer/SuraPawnPlayer.h"
#include "Characters/PawnBasePlayer/SuraPlayerController.h"
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
		SuraPlayerController = Cast<ASuraPlayerController>(PawnOwner->GetController());
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

	if (CurrentMovementState != EMovementState::EMS_WallRun)
	{
		if (SuraPlayerController->GetControlRotation().Roll != 0.f)
		{
			FRotator CurrentControlRotation = SuraPawnPlayer->GetControlRotation();
			FRotator NewRotation = FMath::RInterpTo(CurrentControlRotation, FRotator(CurrentControlRotation.Pitch, CurrentControlRotation.Yaw, 0.f), DeltaTime, 7.f);
			SuraPlayerController->SetControlRotation(NewRotation);
		}
	}
	
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
		if (Velocity.Z >= 0.f)
		{
			WallRunEnterMode = EWallRunEnter::EWRE_Upward;
		}
		else if (Velocity.Z < -800.f)
		{
			WallRunEnterMode = EWallRunEnter::EWRE_Downward;
		}
		else
		{
			WallRunEnterMode = EWallRunEnter::EWRE_Neutral;
		}
		
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
		float MaxHorizontalSpeed = bWallJumpAirBoost ? WallRunJumpAirSpeed2D : RunSpeed;
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

	Velocity.Z = FMath::Max(Velocity.Z - GravityScale * DeltaTime, -MaxFallVerticalSpeed);
	

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

	FVector WallRunDir;
	if (CurrentWallRunSide == EWallRunSide::EWRS_Left)
	{
		WallRunDir = FVector::CrossProduct(CurrentWallHit.ImpactNormal, FVector::UpVector).GetSafeNormal();
	}
	else if (CurrentWallRunSide == EWallRunSide::EWRS_Right)
	{
		WallRunDir = FVector::CrossProduct(CurrentWallHit.ImpactNormal, FVector::DownVector).GetSafeNormal();
	}
	
	float Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(SuraPawnPlayer->GetActorForwardVector(), WallRunDir)));
	float Alpha = FMath::Abs(Angle / 180.f);
	float TargetRoll = 0.f;
	
	if (CurrentWallRunSide == EWallRunSide::EWRS_Left)
	{
		TargetRoll = FMath::Lerp(15, -15, Alpha);
	}
	else if (CurrentWallRunSide == EWallRunSide::EWRS_Right)
	{
		TargetRoll = FMath::Lerp(-15, 15, Alpha);
	}

	FRotator CurrentControlRotation = SuraPawnPlayer->GetControlRotation();
	FRotator NewRotation = FMath::RInterpTo(CurrentControlRotation, FRotator(CurrentControlRotation.Pitch, CurrentControlRotation.Yaw, TargetRoll), DeltaTime, 5.f);
	SuraPlayerController->SetControlRotation(NewRotation);

	if (WallRunEnterMode == EWallRunEnter::EWRE_Upward && bIsDeceleratingZ)
	{
		Velocity.Z = FMath::Max(Velocity.Z - GravityScale * DeltaTime, 0.f);
		if (Velocity.Z == 0.f)
		{
			bIsDeceleratingZ = false;
		}
	}
	else if (WallRunEnterMode == EWallRunEnter::EWRE_Downward && bIsDeceleratingZ)
	{
		Velocity.Z = FMath::Min(Velocity.Z + GravityScale * DeltaTime, 0.f);
		if (Velocity.Z == 0.f)
		{
			bIsDeceleratingZ = false;
		}
	}
	
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
		// Always accelerate in the forward wall run direction when pressing W
		float CurrentSpeed = Velocity.Size2D();
		float NewSpeed;
		if (CurrentSpeed > WallRunMaxSpeed)
		{
			NewSpeed = FMath::Max(CurrentSpeed - WallRunDeceleration * DeltaTime, WallRunMaxSpeed);
		}
		else
		{
			NewSpeed = FMath::Min(CurrentSpeed + WallRunAcceleration * DeltaTime, WallRunMaxSpeed);
		}
    
		// Always set velocity in forward wall run direction when pressing W
		Velocity.X = NewSpeed * VelocityDir.X;
		Velocity.Y = NewSpeed * VelocityDir.Y;
		
	}
	// Input S Key (Backward)
	else if (MovementInputVector.Y < 0.f)
	{
		// Always accelerate in the backward wall run direction when pressing S
		float CurrentSpeed = Velocity.Size2D();
		float NewSpeed;
		if (CurrentSpeed > WallRunBackwardMaxSpeed)
		{
			NewSpeed = FMath::Max(CurrentSpeed - WallRunDeceleration * DeltaTime, WallRunBackwardMaxSpeed);
		}
		else
		{
			NewSpeed = FMath::Min(CurrentSpeed + WallRunAcceleration * DeltaTime, WallRunBackwardMaxSpeed);
		}
    
		// Always set velocity in backward wall run direction when pressing S
		Velocity.X = NewSpeed * -VelocityDir.X;
		Velocity.Y = NewSpeed * -VelocityDir.Y;
	}
	else
	{
	    float NewSpeed = FMath::Max(Velocity.Size2D() - WallRunDeceleration * DeltaTime, 0.f);
	    Velocity.X = VelocityDir.X * NewSpeed;
		Velocity.Y = VelocityDir.Y * NewSpeed;
		if (!bIsDeceleratingZ)
		{
			Velocity.Z = FMath::Max(Velocity.Z - 0.2f  * GravityScale * DeltaTime, -300.f);
		}
	}

	if (!bIsDeceleratingZ && MovementInputVector.Y != 0.f)
	{
		Velocity.Z = 0.f;
	}

	if (bJumpPressed && CurrentJumpCount < MaxJumpCount)
	{
		bJumpPressed = false;
		CurrentJumpCount++;
		FVector WallNormal2D = CurrentWallHit.ImpactNormal.GetSafeNormal2D();
		Velocity = FVector(Velocity.X, Velocity.Y, 0.f) + WallNormal2D * 500.f + FVector::UpVector * JumpZVelocity;
		SetMovementState(EMovementState::EMS_Airborne);
		return;
	}

	if (bCrouchPressed)
	{
		Velocity += CurrentWallHit.ImpactNormal * 100.f;
		SetMovementState(EMovementState::EMS_Airborne);
		return;
	}

	if (IsGrounded())
	{
		SetMovementState(EMovementState::EMS_Move);
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

	if (bWallRightHit && WallRightHit.bBlockingHit && WallRightHit.ImpactNormal.Z < MinWalkableFloorZ &&
		FVector::DotProduct(Velocity.GetSafeNormal2D(), WallRightHit.ImpactNormal.GetSafeNormal2D()) < 0.f)
	{
		bRightWallRunnable = true;
	}

	FHitResult WallLeftHit;
	bool bWallLeftHit = GetWorld()->LineTraceSingleByChannel(WallLeftHit, WallTraceStart, WallTraceLeftEnd,
															 ECC_WorldStatic, WallQueryParams);

	bool bLeftWallRunnable = false;

	if (bWallLeftHit && WallLeftHit.bBlockingHit && WallLeftHit.ImpactNormal.Z < MinWalkableFloorZ &&
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
		bWallJumpAirBoost = false;
		ElapsedTimeFromSurface = 0.f;
		CurrentJumpCount = 0;
	}
	

	if (NewState == EMovementState::EMS_WallRun)
	{
		Velocity.X = Velocity.GetSafeNormal2D().X * WallRunEnterSpeed2D;
		Velocity.Y = Velocity.GetSafeNormal2D().Y * WallRunEnterSpeed2D;

		if (WallRunEnterMode == EWallRunEnter::EWRE_Upward)
		{
			bIsDeceleratingZ = true;
			Velocity.Z = 700.f;
		}
		else if (WallRunEnterMode == EWallRunEnter::EWRE_Downward)
		{
			bIsDeceleratingZ = true;
			Velocity.Z = -700.f;
		}
		else if (WallRunEnterMode == EWallRunEnter::EWRE_Neutral)
		{
			bIsDeceleratingZ = false;
			Velocity.Z = 0.f;
		}
	}

	if (OldState == EMovementState::EMS_WallRun)
	{
		WallRunElapsedTime = 0.f;
		bIsDeceleratingZ = false;

		if (NewState == EMovementState::EMS_Airborne)
		{
			bWallJumpAirBoost = true;
		}
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
		FCollisionShape::MakeSphere(15.f), GroundSweepParams);
	

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




