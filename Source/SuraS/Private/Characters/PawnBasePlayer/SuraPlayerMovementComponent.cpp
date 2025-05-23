// Fill out your copyright notice in the Description page of Project Settings.

// TODO: Make and assign custom trace channels for geometry
// TODO: WallRun 90 degree case handling, WallRun camera yaw handling


#include "Characters/PawnBasePlayer/SuraPlayerMovementComponent.h"

#include "KismetTraceUtils.h"
#include "Characters/PawnBasePlayer/PawnPlayerMovmentRow.h"
#include "Characters/PawnBasePlayer/SuraPawnPlayer.h"
#include "Characters/PawnBasePlayer/SuraPlayerController.h"
#include "Components/CapsuleComponent.h"

#define WALL_TRACE_CHANNEL ECC_GameTraceChannel2

USuraPlayerMovementComponent::USuraPlayerMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickGroup = TG_DuringPhysics;

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

	InitMovementData();

	GravityAcceleration = GravityDirection * GravityScale;
	
	// Start as airborne state
	CurrentMovementState = EMovementState::EMS_Airborne;
	OnAirborne.Broadcast();
	
	PrimaryJumpZVelocity = FMath::Sqrt(2 * GravityScale * PrimaryJumpHeight);
	DoubleJumpZVelocity = FMath::Sqrt(2 * GravityScale * DoubleJumpHeight);
	WallJumpZVelocity = FMath::Sqrt(2 * GravityScale * WallJumpHeight);

}

void USuraPlayerMovementComponent::InitMovementData()
{
	FPawnPlayerMovementRow* Row = MovementDataTable->FindRow<FPawnPlayerMovementRow>("Player", "");
	if (!Row) return;
	GravityScale = Row->GravityScale;
	WalkSpeed = Row->WalkSpeed;
	DashStartSpeed = Row->DashStartSpeed;
	DashEndSpeed = Row->DashEndSpeed;
	DashDecelerationTime = Row->DashDecelerationTime;
	DashCooldown = Row->DashCooldown;
	RunSpeed = Row->RunSpeed;
	CrouchSpeed = Row->CrouchSpeed;
	CrouchHeightScale = Row->CrouchHeightScale;
	PrimaryJumpHeight = Row->PrimaryJumpHeight;
	DoubleJumpHeight = Row->DoubleJumpHeight;
	WallJumpHeight = Row->WallJumpHeight;
	Acceleration = Row->Acceleration;
	Deceleration = Row->Deceleration;
	AirDirectionInterpSpeed = Row->AirDirectionInterpSpeed;
	AirAcceleration = Row->AirAcceleration;
	AirDeceleration = Row->AirDeceleration;
	MaxFallVerticalSpeed = Row->MaxFallVerticalSpeed;
	MaxWalkableFloorAngle = Row->MaxWalkableFloorAngle;
	MaxStepHeight = Row->MaxStepHeight;
	WallRunMaxDuration = Row->WallRunMaxDuration;
	WallRunAcceleration = Row->WallRunAcceleration;
	WallRunDeceleration = Row->WallRunDeceleration;
	WallRunMaxSpeed = Row->WallRunMaxSpeed;
	WallRunBackwardMaxSpeed = Row->WallRunBackwardMaxSpeed;
	WallRunJumpAirSpeed2D = Row->WallRunJumpAirSpeed2D;
	SlideInitialWindow = Row->SlideInitialWindow;
	SlideMaxDuration = Row->SlideMaxDuration;
}



void USuraPlayerMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
                                                 FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	GEngine->AddOnScreenDebugMessage(
		0, 0.f, FColor::Cyan, FString::Printf(TEXT("Current State : %s"), *UEnum::GetValueAsString(CurrentMovementState)));

	GEngine->AddOnScreenDebugMessage(
		1, 0.f, FColor::Cyan, FString::Printf(TEXT("Velocity : (%f, %f, %f) / Size : %f / Size2D : %f "), Velocity.X, Velocity.Y, Velocity.Z, Velocity.Size(), Velocity.Size2D()));

	

	

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

void USuraPlayerMovementComponent::CrouchCapsule(float DeltaTime)
{
	UCapsuleComponent* PlayerCapsule = SuraPawnPlayer->GetCapsuleComponent();
	if (!PlayerCapsule) return;
	float CurrentHalfHeight = PlayerCapsule->GetScaledCapsuleHalfHeight();
	if (CurrentHalfHeight > CrouchCapsuleHalfHeight)
	{
		float NewHeight = FMath::FInterpTo(CurrentHalfHeight, CrouchCapsuleHalfHeight, DeltaTime, 7.f);
		float HeightDelta = CurrentHalfHeight - NewHeight;
		PlayerCapsule->SetCapsuleHalfHeight(NewHeight);
		SuraPawnPlayer->AddActorWorldOffset(FVector(0, 0, -HeightDelta));
	}
}

void USuraPlayerMovementComponent::UnCrouchCapsule(float DeltaTime)
{
	UCapsuleComponent* PlayerCapsule = SuraPawnPlayer->GetCapsuleComponent();
	float CurrentHalfHeight = PlayerCapsule->GetScaledCapsuleHalfHeight();
	if (CurrentHalfHeight < DefaultCapsuleHalfHeight)
	{
		float NewHeight = FMath::FInterpTo(CurrentHalfHeight, DefaultCapsuleHalfHeight, DeltaTime, 7.f);
		float HeightDelta = NewHeight - CurrentHalfHeight;
		SuraPawnPlayer->AddActorWorldOffset(FVector(0, 0, HeightDelta));
		PlayerCapsule->SetCapsuleHalfHeight(NewHeight);
		
	}
}

void USuraPlayerMovementComponent::TickMove(float DeltaTime)
{

	if (!IsGrounded())
	{
		if (CurrentJumpCount > 0)
		{
			CurrentJumpCount--;
		}
		OnAirborne.Broadcast();
		SetMovementState(EMovementState::EMS_Airborne);
		return;
	}
	
	const FVector InputDirection = ConsumeInputVector().GetSafeNormal();

	if (bCrouchPressed)
	{

		bIsCrouching = true;
		
		if (Velocity.Size() >= RunSpeed - 50.f && GroundHit.ImpactNormal.Z >= MinWalkableFloorZ)
		{
			SlideStartDirection = FVector::VectorPlaneProject(Velocity, GroundHit.ImpactNormal).GetSafeNormal();
			Velocity = bHasRecentlySlid ? SlideStartDirection * Velocity.Size() : SlideStartDirection * (Velocity.Size() + 700.f);
			bIsDashing = false;
			ElapsedTimeFromDash = 0.f;
			SlideResetTimer = 0.;
			
			OnSlide.Broadcast();
			SetMovementState(EMovementState::EMS_Slide);
			return;
		}

		CrouchCapsule(DeltaTime);
	}
	else
	{
		float CurrentCapsuleHalfHeight = SuraPawnPlayer->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		if (GetWorld() && CurrentCapsuleHalfHeight < DefaultCapsuleHalfHeight)
		{
			FHitResult UnCrouchHitResult;
			FCollisionQueryParams UnCrouchParams;
			UnCrouchParams.AddIgnoredActor(SuraPawnPlayer);

			

			FVector UnCrouchSweepStart = SuraPawnPlayer->GetActorLocation();
			// (DefaultHalfHeight - CurrentHalfHeight) for default height assumed actor location adjustment
			// (DefaultHalfHeight - DefaultRadius) for default capsule half height excluding hemisphere
			// 5.f for offset
			// (DefaultHalfHeight - CurrentHalfHeight) + (DefaultHalfHeight - DefaultRadius) + 5.f
			// There is a built-in function to get the half height without hemisphere but didn't want to make another member var
			FVector UnCrouchSweepEnd = UnCrouchSweepStart + FVector::UpVector * (2 * DefaultCapsuleHalfHeight - CurrentCapsuleHalfHeight - DefaultCapsuleRadius + 5.f); 
			
			bool bUnCrouchHit = GetWorld()->SweepSingleByChannel(
				UnCrouchHitResult,
				UnCrouchSweepStart,
				UnCrouchSweepEnd,
				FQuat::Identity,
				ECC_Visibility,
				FCollisionShape::MakeSphere(DefaultCapsuleRadius),
				UnCrouchParams);

			if (!bUnCrouchHit)
			{
				bIsCrouching = false;

				UnCrouchCapsule(DeltaTime);
			}
			
		}
	}

	if (bHasRecentlySlid)
	{
		SlideResetTimer += DeltaTime;
		if (SlideResetTimer >= SlideBoostResetDelay)
		{
			bHasRecentlySlid = false;
			SlideElapsedTime = 0.f;
			SlideResetTimer = 0.;
		}
	}

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
			float WishSpeed = bIsCrouching ? CrouchSpeed : (MovementInputVector.Y > 0.f ? RunSpeed : WalkSpeed);
			
			FVector AcceleratedVelocity = Velocity + InputDirection * Acceleration * DeltaTime;
			Velocity = AcceleratedVelocity.Size() > WishSpeed ? AcceleratedVelocity.GetSafeNormal() * WishSpeed : AcceleratedVelocity;
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

			if (FVector::DotProduct(InputDirection, Velocity.GetSafeNormal2D()) < 0.f)
			{
				bIsDashing = false;
				ElapsedTimeFromDash = 0.f;
				Velocity = FVector::ZeroVector;
			}
		}
		else
		{
			bIsDashing = false;
			ElapsedTimeFromDash = 0.f;
			Velocity = Velocity.GetSafeNormal() * DashEndSpeed;
		}
	}

	

	if (!bIsDashing)
	{
		if (GroundHit.ImpactNormal.Z >= MinWalkableFloorZ)
		{
			Velocity = FVector::VectorPlaneProject(Velocity, GroundHit.ImpactNormal).GetSafeNormal() * Velocity.Size();
		}
	}

	TArray<FHitResult> StepHits;
	FCollisionQueryParams StepParams;
	StepParams.AddIgnoredActor(SuraPawnPlayer);

	FVector StepTraceStart = SuraPawnPlayer->GetActorLocation() + FVector(0, 0, -1) * (SuraPawnPlayer->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() - 5.f);
	FVector StepTraceEnd = StepTraceStart + FVector::VectorPlaneProject(InputDirection, GroundHit.ImpactNormal).GetSafeNormal() * SuraPawnPlayer->GetCapsuleComponent()->GetScaledCapsuleRadius();
	FCollisionShape StepShape = FCollisionShape::MakeSphere(5.f);
	bool bStepHit = GetWorld()->SweepMultiByChannel(StepHits, StepTraceStart, StepTraceEnd, FQuat::Identity, ECC_WorldStatic, StepShape, StepParams);

	// DrawDebugSphereTraceMulti(GetWorld(), StepTraceStart, StepTraceEnd,
	// 	5.f, EDrawDebugTrace::ForDuration, bStepHit, StepHits, FLinearColor::Red, FLinearColor::Green, 1.f);
	

	if (bStepHit)
	{
		FHitResult StepWall;
		bool bFoundStepWall = false;
		for (const FHitResult& StepHit : StepHits)
		{
			if (StepHit.IsValidBlockingHit() && StepHit.ImpactNormal.Z < MinWalkableFloorZ)
			{
				bFoundStepWall = true;
				StepWall = StepHit;
				break;
			}
		}

		if (bFoundStepWall)
		{
			FCollisionShape CapsuleShape = SuraPawnPlayer->GetCapsuleComponent()->GetCollisionShape();
			FPlane GroundPlane(GroundHit.ImpactPoint, GroundHit.ImpactNormal);
			float PlanePointZ = (GroundPlane.W - GroundPlane.X * StepWall.ImpactPoint.X - GroundPlane.Y * StepWall.ImpactPoint.Y) / GroundPlane.Z;
			float StepHeightZ = PlanePointZ + CapsuleShape.GetCapsuleHalfHeight() + MaxStepHeight;
			
			FVector DownStepTraceStart = 
				FVector(StepWall.ImpactPoint.X, StepWall.ImpactPoint.Y, StepHeightZ) +
				FVector::VectorPlaneProject(InputDirection, GroundHit.ImpactNormal).GetSafeNormal() * 3.f;
			FVector DownStepTraceEnd = DownStepTraceStart + FVector(0, 0, -1) * (StepHeightZ + 10.f);

			FHitResult StepFloorHit;
			FCollisionQueryParams StepFloorParams;
			StepFloorParams.AddIgnoredActor(SuraPawnPlayer);

			bool bStepFloorHit = GetWorld()->SweepSingleByChannel(StepFloorHit, DownStepTraceStart, DownStepTraceEnd, FQuat::Identity, ECC_WorldStatic,
				CapsuleShape, StepFloorParams);

			// DrawDebugCapsuleTraceSingle(GetWorld(), DownStepTraceStart, DownStepTraceEnd, 40.f, 90.f, EDrawDebugTrace::ForDuration, bStepFloorHit,
			// 	StepFloorHit, FLinearColor::Green, FLinearColor::Red, 1.f);

			if (bStepFloorHit && StepFloorHit.IsValidBlockingHit() && StepFloorHit.ImpactNormal.Z >= MinWalkableFloorZ)
			{
				SuraPawnPlayer->SetActorLocation(StepFloorHit.Location);
			}
		}
	}

	

	if (bJumpPressed && CurrentJumpCount < MaxJumpCount)
	{
		bJumpPressed = false;
		CurrentJumpCount++;
		Velocity.Z = PrimaryJumpZVelocity;

		if (bIsDashing) bHasDashedInAir = true;
		
		OnPrimaryJump.Broadcast();
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
		OnDash.Broadcast(MovementInputVector);
	}
	
}

void USuraPlayerMovementComponent::TickSlide(float DeltaTime)
{
	if (!IsGrounded() || GroundHit.ImpactNormal.Z < MinWalkableFloorZ)
	{
		OnAirborne.Broadcast();
		SetMovementState(EMovementState::EMS_Airborne);
		return;
	}

	if (!bCrouchPressed)
	{
		bIsCrouching = false;
		OnMove.Broadcast();
		SetMovementState(EMovementState::EMS_Move);
		return;
	}

	SlideStateElapsedTime += DeltaTime;

	CrouchCapsule(DeltaTime);
	
	bool bIsSlidingDown = false;
	FVector DownwardDirection = FVector::VectorPlaneProject(FVector::DownVector, GroundHit.ImpactNormal).GetSafeNormal();
	if (DownwardDirection.IsZero())
	{
		DownwardDirection = FVector::VectorPlaneProject(Velocity, GroundHit.ImpactNormal).GetSafeNormal();
		bIsSlidingDown = false;
	}
	else
	{
		FVector ProjectedVelocity = FVector::VectorPlaneProject(Velocity, GroundHit.ImpactNormal).GetSafeNormal();
		if (FVector::DotProduct(ProjectedVelocity, DownwardDirection) >= 0.f)
		{
			bIsSlidingDown = true;
		}
		else
		{
			bIsSlidingDown = false;
		}
	}
	
	FVector InterpDirection = FMath::VInterpNormalRotationTo(Velocity.GetSafeNormal(), DownwardDirection, DeltaTime, 40.f);
	FVector ProjectedSlideStartDirection = FVector::VectorPlaneProject(SlideStartDirection, GroundHit.ImpactNormal).GetSafeNormal();
	FVector SlideDirection = SlideStateElapsedTime > SlideInitialWindow ? InterpDirection : ProjectedSlideStartDirection;
	

	if (bIsSlidingDown)
	{
		Velocity = Velocity.Size() * SlideDirection;
	}
	else
	{
		if (SlideElapsedTime + DeltaTime < SlideMaxDuration)
		{
			SlideElapsedTime += DeltaTime;
		}
		else
		{
			bHasRecentlySlid = false;
			SlideElapsedTime = 0.f;
		}
		
		float DecelerationAmount = SlideDecelerationAmount * DeltaTime;
		float NewSpeed = Velocity.Size() - DecelerationAmount;
		if (NewSpeed <= CrouchSpeed)
		{
			Velocity = CrouchSpeed * SlideDirection;
			bHasRecentlySlid = false;
			SlideElapsedTime = 0.f;
			SetMovementState(EMovementState::EMS_Move);
			return;
		}

		Velocity = NewSpeed * SlideDirection; 
	}

	if (bJumpPressed && CurrentJumpCount < MaxJumpCount)
	{
		bJumpPressed = false;
		CurrentJumpCount++;
		Velocity.Z = PrimaryJumpZVelocity;
		OnPrimaryJump.Broadcast();
		SetMovementState(EMovementState::EMS_Airborne);
		return;
	}
	
	
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
			if (GroundHit.ImpactNormal.Z >= MinWalkableFloorZ)
			{
				Velocity = FVector::VectorPlaneProject(Velocity, GroundHit.ImpactNormal).GetSafeNormal() * RunSpeed;
			}
			OnLand.Broadcast(Velocity.Z);
			OnMove.Broadcast();
			SetMovementState(EMovementState::EMS_Move);
			return;
		}
		else
		{
			if (ElapsedTimeFromSurface > JumpBuffer)
			{
				if (GroundHit.ImpactNormal.Z >= MinWalkableFloorZ)
				{
					Velocity = FVector::VectorPlaneProject(Velocity, GroundHit.ImpactNormal).GetSafeNormal() * RunSpeed;
				}
				OnLand.Broadcast(Velocity.Z);
				OnMove.Broadcast();
				SetMovementState(EMovementState::EMS_Move);
				return;
			}
		}
	}

	if (bCrouchPressed)
	{
	
		bIsCrouching = true;
		
		CrouchCapsule(DeltaTime);
	}
	else
	{
		float CurrentCapsuleHalfHeight = SuraPawnPlayer->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		if (GetWorld() && CurrentCapsuleHalfHeight < DefaultCapsuleHalfHeight)
		{
			FHitResult UnCrouchHitResult;
			FCollisionQueryParams UnCrouchParams;
			UnCrouchParams.AddIgnoredActor(SuraPawnPlayer);
	
			
	
			FVector UnCrouchSweepStart = SuraPawnPlayer->GetActorLocation();
			// (DefaultHalfHeight - CurrentHalfHeight) for default height assumed actor location adjustment
			// (DefaultHalfHeight - DefaultRadius) for default capsule half height excluding hemisphere
			// 5.f for offset
			// (DefaultHalfHeight - CurrentHalfHeight) + (DefaultHalfHeight - DefaultRadius) + 5.f
			// There is a built-in function to get the half height without hemisphere but didn't want to make another member var
			FVector UnCrouchSweepEnd = UnCrouchSweepStart + FVector::UpVector * (2 * DefaultCapsuleHalfHeight - CurrentCapsuleHalfHeight - DefaultCapsuleRadius + 5.f); 
			
			bool bUnCrouchHit = GetWorld()->SweepSingleByChannel(
				UnCrouchHitResult,
				UnCrouchSweepStart,
				UnCrouchSweepEnd,
				FQuat::Identity,
				ECC_Visibility,
				FCollisionShape::MakeSphere(DefaultCapsuleRadius),
				UnCrouchParams);
	
			if (!bUnCrouchHit)
			{
				bIsCrouching = false;
	
				UnCrouchCapsule(DeltaTime);
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
			bIsDashing = false;
			ElapsedTimeFromDash = 0.f;
			OnWallRun.Broadcast();
			SetMovementState(EMovementState::EMS_WallRun);
			return;
		}
		else
		{
			if (ElapsedTimeFromSurface > WallJumpBuffer)
			{
				bIsDashing = false;
				ElapsedTimeFromDash = 0.f;
				OnWallRun.Broadcast();
				SetMovementState(EMovementState::EMS_WallRun);
				return;
			}
		}
	}
	
	const FVector InputDirection = ConsumeInputVector().GetSafeNormal();

	// Mantle Sweep Section
	FCollisionShape PlayerCapsule = SuraPawnPlayer->GetCapsuleComponent()->GetCollisionShape();
	FCollisionQueryParams MantleParams;
	MantleParams.AddIgnoredActor(SuraPawnPlayer);
	FVector MantleSweepEnd = SuraPawnPlayer->GetActorLocation() + SuraPawnPlayer->GetActorForwardVector() * 75.f;
	bool bMantleWallHit = GetWorld()->SweepSingleByChannel(MantleWallHit, SuraPawnPlayer->GetActorLocation(),
		MantleSweepEnd, FQuat::Identity, WALL_TRACE_CHANNEL, PlayerCapsule, MantleParams);

	if (bMantleWallHit && MantleWallHit.bBlockingHit && MantleWallHit.ImpactNormal.Z < MinWalkableFloorZ)
	{
		if (MovementInputVector.Y > 0.f && !bCrouchPressed)
		{
			// TODO: Make Mantle Available Height as a variable 50.f
			FVector FloorHitStart = SuraPawnPlayer->GetActorLocation() + SuraPawnPlayer->GetActorForwardVector() * 75.f +
				FVector::UpVector * (PlayerCapsule.GetCapsuleHalfHeight() + 100.f);
			FVector FloorHitEnd = FloorHitStart + FVector::DownVector * (2 * PlayerCapsule.GetCapsuleHalfHeight() + 100.f); 
			bool bMantleFloorHit = GetWorld()->SweepSingleByChannel(MantleFloorHit, FloorHitStart, FloorHitEnd, FQuat::Identity, ECC_WorldStatic, PlayerCapsule,
				MantleParams);

			DrawDebugCapsuleTraceSingle(GetWorld(), FloorHitStart, FloorHitEnd, PlayerCapsule.GetCapsuleRadius(),
				PlayerCapsule.GetCapsuleHalfHeight(), EDrawDebugTrace::ForDuration, bMantleFloorHit && MantleFloorHit.IsValidBlockingHit(), MantleFloorHit, FLinearColor::Red, FLinearColor::Green, 1.f);

			if (bMantleFloorHit && MantleFloorHit.IsValidBlockingHit() && MantleFloorHit.ImpactNormal.Z >= MinWalkableFloorZ)
			{
				bIsDashing = false;
				ElapsedTimeFromDash = 0.f;
				OnMantle.Broadcast();
				SetMovementState(EMovementState::EMS_Mantle);
				return;
			}
		}
	}

	if (!bIsDashing)
	{
		// float MaxHorizontalSpeed = bWallJumpAirBoost ? WallRunJumpAirSpeed2D : RunSpeed;

		float MaxHorizontalSpeed = RunSpeed;
		if (bWallJumpAirBoost)
		{
			MaxHorizontalSpeed = WallRunJumpAirSpeed2D;
		}
		else if (bHasDashedInAir)
		{
			MaxHorizontalSpeed = DashEndSpeed;
		}
		
		if (Velocity.Size2D() > MaxHorizontalSpeed)
		{
			if (!InputDirection.IsNearlyZero()) 
			{
				FVector CurrentDir2D = FVector(Velocity.X, Velocity.Y, 0.f).GetSafeNormal(); 
				FVector TargetDir2D = InputDirection;  

				// Interpolate current direction towards target input direction
				FVector NewDirection = FMath::VInterpTo(CurrentDir2D, TargetDir2D, DeltaTime, AirDirectionInterpSpeed); 
				
				Velocity.X = NewDirection.X * MaxHorizontalSpeed;
				Velocity.Y = NewDirection.Y * MaxHorizontalSpeed;
			}
			
			// FVector VelocityXY = FVector(Velocity.X, Velocity.Y, 0.f);
			// FVector NewVelocityXY = VelocityXY - VelocityXY.GetSafeNormal() * AirDeceleration * DeltaTime;
			// if (NewVelocityXY.Size() < MaxHorizontalSpeed)
			// {
			// 	NewVelocityXY = NewVelocityXY.GetSafeNormal() * MaxHorizontalSpeed;
			// }
			//
			// Velocity.X = NewVelocityXY.X;
			// Velocity.Y = NewVelocityXY.Y;
		}
		else
		{
			FVector VelocityXY = FVector(Velocity.X, Velocity.Y, 0.f);
			FVector NewVelocityXY = VelocityXY + InputDirection * AirAcceleration * DeltaTime;
			if (NewVelocityXY.Size() > MaxHorizontalSpeed)
			{
				NewVelocityXY = NewVelocityXY.GetSafeNormal() * MaxHorizontalSpeed;
			}
			
			Velocity.X = NewVelocityXY.X;
			Velocity.Y = NewVelocityXY.Y;
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
			bHasRecentlySlid = false;
			SlideElapsedTime = 0.f;
			CurrentJumpCount++;
			OnDoubleJump.Broadcast();
			Velocity.Z = DoubleJumpZVelocity;
		}
	}

	if (bDashPressed && AvailableDashCount > 0)
	{
		
		bDashPressed = false;
		bIsDashing = true;
		bHasDashedInAir = true;
		
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
		Velocity = DashDirection.GetSafeNormal2D() * DashStartSpeed + FVector(0, 0, Velocity.Z);
		OnDash.Broadcast(MovementInputVector);
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
		OnAirborne.Broadcast();
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
		OnAirborne.Broadcast();
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
	    	OnAirborne.Broadcast();
	        SetMovementState(EMovementState::EMS_Airborne);
	        return;
	    }

		float WallRunViewAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(VelocityDir, SuraPawnPlayer->GetActorForwardVector())));
		bool bIsRight = FVector::CrossProduct(VelocityDir, SuraPawnPlayer->GetActorForwardVector()).GetSafeNormal().Z > 0;

		if (WallRunViewAngle > 60.f && bIsRight)
		{
			Velocity += CurrentWallHit.ImpactNormal * 100.f;
			OnAirborne.Broadcast();
			SetMovementState(EMovementState::EMS_Airborne);
			return;
		}

		if (FMath::FindDeltaAngleDegrees(VelocityDir.Rotation().Yaw, SuraPawnPlayer->GetControlRotation().Yaw) < 0)
		{
			SuraPawnPlayer->AddControllerYawInput(20.f * DeltaTime);
		}
	}
	else // EWRS_Right
	{
	    VelocityDir = FVector::CrossProduct(CurrentWallHit.ImpactNormal, FVector::DownVector).GetSafeNormal();
	    
	    // Check if player wants to exit wall to the left
	    if (MovementInputVector.X < 0.f)
	    {
	    	Velocity += CurrentWallHit.ImpactNormal * 100.f;
	    	OnAirborne.Broadcast();
	        SetMovementState(EMovementState::EMS_Airborne);
	        return;
	    }

		float WallRunViewAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(VelocityDir, SuraPawnPlayer->GetActorForwardVector())));
		bool bIsLeft = FVector::CrossProduct(VelocityDir, SuraPawnPlayer->GetActorForwardVector()).GetSafeNormal().Z < 0;

		if (WallRunViewAngle > 60.f && bIsLeft)
		{
			Velocity += CurrentWallHit.ImpactNormal * 100.f;
			OnAirborne.Broadcast();
			SetMovementState(EMovementState::EMS_Airborne);
			return;
		}

		if (FMath::FindDeltaAngleDegrees(VelocityDir.Rotation().Yaw, SuraPawnPlayer->GetControlRotation().Yaw) > 0)
		{
			SuraPawnPlayer->AddControllerYawInput(-20.f * DeltaTime);
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
		// // Always accelerate in the backward wall run direction when pressing S
		// float CurrentSpeed = Velocity.Size2D();
		// float NewSpeed;
		// if (CurrentSpeed > WallRunBackwardMaxSpeed)
		// {
		// 	NewSpeed = FMath::Max(CurrentSpeed - WallRunDeceleration * DeltaTime, WallRunBackwardMaxSpeed);
		// }
		// else
		// {
		// 	NewSpeed = FMath::Min(CurrentSpeed + WallRunAcceleration * DeltaTime, WallRunBackwardMaxSpeed);
		// }
  //   
		// // Always set velocity in backward wall run direction when pressing S
		// Velocity.X = NewSpeed * -VelocityDir.X;
		// Velocity.Y = NewSpeed * -VelocityDir.Y;

		// Calculate if we're already moving backward along the wall
		bool bIsMovingBackward = (FVector2D::DotProduct(FVector2D(Velocity), FVector2D(VelocityDir)) < 0);
		float CurrentSpeed = Velocity.Size2D();
		float NewSpeed;

		// If already moving backward and above max speed
		if (bIsMovingBackward && CurrentSpeed > WallRunBackwardMaxSpeed)
		{
			// Allow a more gradual deceleration when entering with high backward speed
			// Apply a smaller deceleration rate initially to preserve momentum
			float InitialDecelRate = WallRunDeceleration * 0.5f;
			NewSpeed = FMath::Max(CurrentSpeed - InitialDecelRate * DeltaTime, WallRunBackwardMaxSpeed);
		}
		// Standard backward acceleration/deceleration
		else if (bIsMovingBackward)
		{
			NewSpeed = FMath::Min(CurrentSpeed + WallRunAcceleration * DeltaTime, WallRunBackwardMaxSpeed);
		}
		// Moving forward but pressing S, decelerate and then reverse
		else
		{
			// First decelerate to zero
			NewSpeed = FMath::Max(CurrentSpeed - WallRunDeceleration * 1.5f * DeltaTime, 0.f);
        
			// If completely decelerated, start moving backward
			if (NewSpeed <= 0.1f)
			{
				NewSpeed = FMath::Min(WallRunAcceleration * 0.5f * DeltaTime, WallRunBackwardMaxSpeed);
				bIsMovingBackward = true;
			}
		}

		// Apply velocity in the correct direction
		if (bIsMovingBackward)
		{
			Velocity.X = NewSpeed * -VelocityDir.X;
			Velocity.Y = NewSpeed * -VelocityDir.Y;
		}
		else
		{
			Velocity.X = NewSpeed * VelocityDir.X;
			Velocity.Y = NewSpeed * VelocityDir.Y;
		}
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
		Velocity = FVector(Velocity.X, Velocity.Y, 0.f) + WallNormal2D * 500.f + FVector::UpVector * WallJumpZVelocity;
		OnWallJump.Broadcast();
		SetMovementState(EMovementState::EMS_Airborne);
		return;
	}

	if (bCrouchPressed)
	{
		Velocity += CurrentWallHit.ImpactNormal * 100.f;
		OnAirborne.Broadcast();
		SetMovementState(EMovementState::EMS_Airborne);
		return;
	}

	if (IsGrounded())
	{
		OnMove.Broadcast();
		SetMovementState(EMovementState::EMS_Move);
		return;
	}
	
}

void USuraPlayerMovementComponent::TickHang(float DeltaTime){}

void USuraPlayerMovementComponent::TickMantle(float DeltaTime)
{
	FVector TargetPos = MantleFloorHit.Location;
	if (FVector::Distance(SuraPawnPlayer->GetActorLocation(), TargetPos) < 5.f)
	{
		Velocity = FVector::ZeroVector;
		OnMove.Broadcast();
		SetMovementState(EMovementState::EMS_Move);
		return;
	}
	
	Velocity = FVector(TargetPos - SuraPawnPlayer->GetActorLocation()).GetSafeNormal() * 800.f;
}

bool USuraPlayerMovementComponent::CanWallRun()
{
	if (bCrouchPressed) return false;
	

	FCollisionQueryParams WallQueryParams;
	WallQueryParams.AddIgnoredActor(SuraPawnPlayer);
	
	FVector WallTraceStart = PawnOwner->GetActorLocation();
	FVector WallTraceRightEnd = WallTraceStart + PawnOwner->GetActorRightVector() * 70.f;
	FVector WallTraceLeftEnd = WallTraceStart - PawnOwner->GetActorRightVector() * 70.f;
	

	FHitResult WallRightHit;
	bool bWallRightHit = GetWorld()->SweepSingleByChannel(WallRightHit, WallTraceStart, WallTraceRightEnd, FQuat::Identity,
															  WALL_TRACE_CHANNEL, FCollisionShape::MakeSphere(10.f), WallQueryParams);

	bool bRightWallRunnable = false;

	if (bWallRightHit && WallRightHit.bBlockingHit && WallRightHit.ImpactNormal.Z < MinWalkableFloorZ &&
		FVector::DotProduct(Velocity.GetSafeNormal2D(), WallRightHit.ImpactNormal.GetSafeNormal2D()) < 0.f)
	{
		bRightWallRunnable = true;
	}

	FHitResult WallLeftHit;
	bool bWallLeftHit = GetWorld()->SweepSingleByChannel(WallLeftHit, WallTraceStart, WallTraceLeftEnd, FQuat::Identity,
															 WALL_TRACE_CHANNEL, FCollisionShape::MakeSphere(10.f), WallQueryParams);

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
	if (OldState == EMovementState::EMS_Slide)
	{
		// This is for slide direction.
		SlideStateElapsedTime = 0.f;
	}

	if (NewState == EMovementState::EMS_Slide)
	{
		bHasRecentlySlid = true;
		SlideDecelerationAmount = (Velocity.Size() - CrouchSpeed) / FMath::Max(SlideMaxDuration - SlideElapsedTime, 0.1f);
	}

	if (OldState == EMovementState::EMS_Move)
	{
		SlideResetTimer = 0.f;
	}

	
	if (OldState == EMovementState::EMS_Airborne)
	{
		bHasDashedInAir = false;
		bWallJumpAirBoost = false;
		ElapsedTimeFromSurface = 0.f;
		CurrentJumpCount = 0;
	}

	if (NewState == EMovementState::EMS_Mantle)
	{
		bHasRecentlySlid = false;
		SlideElapsedTime = 0.f;
	}
	

	if (NewState == EMovementState::EMS_WallRun)
	{
		bHasRecentlySlid = false;
		SlideElapsedTime = 0.f; 
		
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





bool USuraPlayerMovementComponent::IsGrounded()
{
	FCollisionQueryParams GroundSweepParams;
	GroundSweepParams.AddIgnoredActor(PawnOwner);
	
	FVector SweepStart = SuraPawnPlayer->GetActorLocation();
	FVector SweepEnd = SuraPawnPlayer->GetActorLocation() +
		FVector::DownVector * (SuraPawnPlayer->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
	
	bool bHit = GetWorld()->SweepSingleByChannel(GroundHit, SweepStart, SweepEnd, FQuat::Identity, ECC_WorldStatic,
		FCollisionShape::MakeSphere(15.f), GroundSweepParams);
	

	if (!bHit || !GroundHit.bBlockingHit)
	{
		return false;
	}

	if (GroundHit.ImpactNormal.Z < MinWalkableFloorZ)
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




