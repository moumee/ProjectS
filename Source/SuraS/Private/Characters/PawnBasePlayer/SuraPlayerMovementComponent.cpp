// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PawnBasePlayer/SuraPlayerMovementComponent.h"

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

	SetMovementState(FGameplayTag::RequestGameplayTag("State.Movement.Airborne.Falling"));
}

void USuraPlayerMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
                                                 FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime))
	{
		return;
	}

	if (!IsGrounded() && MovementStateTag.MatchesTag(FGameplayTag::RequestGameplayTag("State.Movement.Grounded")))
	{
		bIsGrounded = false;
		if (Velocity.Z < 0.f)
		{
			SetMovementState(FGameplayTag::RequestGameplayTag("State.Movement.Airborne.Falling"));
		}
		else
		{
			SetMovementState(FGameplayTag::RequestGameplayTag("State.Movement.Airborne.Jumping"));
		}
	}

	if (bIsGrounded)
	{
		
	}
	
	FVector DeltaVelocity = ConsumeInputVector().GetClampedToMaxSize(1.f) * Acceleration * DeltaTime;
	FVector SummedVelocity = Velocity + DeltaVelocity;
	FVector SummedVelocityXY = FVector(SummedVelocity.X, SummedVelocity.Y, 0);
	if (SummedVelocityXY.Size() > WishSpeed)
	{
		FVector ClampedXY = SummedVelocityXY.GetClampedToMaxSize(WishSpeed);
		Velocity = ClampedXY + Velocity.Z;
	}
	else
	{
		Velocity = SummedVelocity;
	}

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

bool USuraPlayerMovementComponent::IsGrounded() const
{
	FHitResult GroundSweepHit;
	FCollisionQueryParams GroundSweepParams;
	GroundSweepParams.AddIgnoredActor(PawnOwner);

	FCollisionShape CapsuleShape = SuraPawnPlayer->GetCapsuleComponent()->GetCollisionShape();

	FVector SweepStart = SuraPawnPlayer->GetActorLocation();
	FVector SweepEnd = SuraPawnPlayer->GetActorLocation() -
		FVector::DownVector * (SuraPawnPlayer->GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 10.f);
	
	bool bHit = GetWorld()->SweepSingleByChannel(GroundSweepHit, SweepStart, SweepEnd, FQuat::Identity, ECC_WorldStatic,
		CapsuleShape, GroundSweepParams);

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

void USuraPlayerMovementComponent::SetMovementState(const FGameplayTag& InStateTag)
{
	MovementStateTag = InStateTag;
    
	TArray<FGameplayTag> TagsToRemove;
    
	for (const FGameplayTag& Tag : OwnedGameplayTags)
	{
		if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag("State.Movement")))
		{
			TagsToRemove.Add(Tag);
		}
	}
    
	for (const FGameplayTag& Tag : TagsToRemove)
	{
		OwnedGameplayTags.RemoveTag(Tag);
	}
	OwnedGameplayTags.AddTag(InStateTag);
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



