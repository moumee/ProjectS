
#include "ActorComponents/WeaponSystem/SuraPlayerAnimInstance_Weapon.h"

#include "Characters/PawnBasePlayer/SuraPawnPlayer.h"

#include "ActorComponents/WeaponSystem/WeaponSystemComponent.h"
#include "ActorComponents/WeaponSystem/ACWeapon.h"
#include "ActorComponents/WeaponSystem/SuraWeaponBaseState.h"

#include "KismetAnimationLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"


void USuraPlayerAnimInstance_Weapon::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	SuraPlayer = Cast<ASuraPawnPlayer>(TryGetPawnOwner());

	if (SuraPlayer)
	{
		// <Character Movement>
		MovementState = SuraPlayer->GetPlayerMovementComponent()->GetMovementState();
		bIsRunning = SuraPlayer->GetPlayerMovementComponent()->IsRunning();
		bIsCrouching = SuraPlayer->GetPlayerMovementComponent()->IsCrouching();
		bIsDashing = SuraPlayer->GetPlayerMovementComponent()->IsDashing();
		Velocity = SuraPlayer->GetPlayerMovementComponent()->Velocity;
		MovementInputVector = SuraPlayer->GetPlayerMovementComponent()->GetMovementInputVector();


		//-------------------------------------------
		// <weapon System>
		SetAimPoint();
		CurrentWeaponStateType = EWeaponStateType::WeaponStateType_None;

		if (SuraPlayer->GetWeaponSystemComponent())
		{
			//SetAimSocket();
		}
	}
}

void USuraPlayerAnimInstance_Weapon::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	if (SuraPlayer)
	{
		// <Character Movement>
		MovementState = SuraPlayer->GetPlayerMovementComponent()->GetMovementState();
		bIsRunning = SuraPlayer->GetPlayerMovementComponent()->IsRunning();
		bIsCrouching = SuraPlayer->GetPlayerMovementComponent()->IsCrouching();
		bIsDashing = SuraPlayer->GetPlayerMovementComponent()->IsDashing();
		Velocity = SuraPlayer->GetPlayerMovementComponent()->Velocity;
		MovementInputVector = SuraPlayer->GetPlayerMovementComponent()->GetMovementInputVector();
	}
}

void USuraPlayerAnimInstance_Weapon::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (SuraPlayer)
	{
		// <Weapon System>
		bIsZoomIn = SuraPlayer->GetWeaponSystemComponent()->IsZoomIn();
		//ScreenCenterWorldLocation = Character->GetWeaponSystem()->GetScreenCenterWorldPosition();
		//TargetRightHandWorldLocation = Character->GetWeaponSystem()->GetTargetRightHandWorldLocation();
		SetTargetRightHandTransform();


		//TODO: Update 말고 트리거로 작동하게 해야함
		//SetAimSocket();
		//SetAimPoint();
		if (SuraPlayer->GetWeaponSystemComponent())
		{
			//SetAimSocket();
			SetAimPoint();
			UpdateWeapon();
			UpdateArmRecoil(DeltaTime);
			ConvertRecoilValueFrame();

			LeftHandTransform = GetLeftHandTransform();
			TargetLeftHandSocketTransform = GetTargetLeftHandTransfrom();
		}

		// DefalutCameraRelativeTransform = Character->GetCamera()->GetRelativeTransform();
		// LogTransform(AimSocketRelativeTransform);
	}
}

void USuraPlayerAnimInstance_Weapon::UpdateWeapon()
{
	if (IsValid(SuraPlayer->GetWeaponSystemComponent())
		&& SuraPlayer->GetWeaponSystemComponent() != nullptr)
	{
		if (SuraPlayer->GetWeaponSystemComponent()->GetWeaponNum() != 0
			&& IsValid(SuraPlayer->GetWeaponSystemComponent()->GetCurrentWeapon())
			&& SuraPlayer->GetWeaponSystemComponent()->GetCurrentWeapon() != nullptr)
		{
			bHasWeapon = true;

			AimSocketRelativeTransform = SuraPlayer->GetWeaponSystemComponent()->GetCurrentWeapon()->GetAimSocketRelativeTransform();

			//-------------------------------------------------------------
			CurrentWeaponStateType = SuraPlayer->GetWeaponSystemComponent()->GetCurrentWeapon()->GetCurrentState()->GetWeaponStateType();

			//if (CurrentWeaponStateType == EWeaponStateType::WeaponStateType_Firing)
			//{
			//	UE_LOG(LogTemp, Warning, TEXT("Firing State!!!!!!!!!!"));
			//}
			//else if (CurrentWeaponStateType == EWeaponStateType::WeaponStateType_Reloading)
			//{
			//	UE_LOG(LogTemp, Warning, TEXT("Reloading State!!!!!!!!!!"));
			//}
			//-------------------------------------------------------------

			if (CurrentWeapon != SuraPlayer->GetWeaponSystemComponent()->GetCurrentWeapon())
			{
				// <RightHandSocket>
				RightHandSocketTransform = SuraPlayer->GetWeaponSystemComponent()->GetCurrentWeapon()->GetRightHandSocketTransform();
				RightHandSocketTransform_Crouch = SuraPlayer->GetWeaponSystemComponent()->GetCurrentWeapon()->GetRightHandSocketTransform_Crouch();

				// <Recoil>
				ArmRecoil = *SuraPlayer->GetWeaponSystemComponent()->GetCurrentWeapon()->GetArmRecoilInfo();
				ArmRecoil_Hand = *SuraPlayer->GetWeaponSystemComponent()->GetCurrentWeapon()->GetArmRecoilInfo_Hand();
				ArmRecoil_UpperArm = *SuraPlayer->GetWeaponSystemComponent()->GetCurrentWeapon()->GetArmRecoilInfo_UpperArm();
				ArmRecoil_LowerArm = *SuraPlayer->GetWeaponSystemComponent()->GetCurrentWeapon()->GetArmRecoilInfo_LowerArm();
			}
		}


		//if (CurrentWeapon != Character->GetWeaponSystem()->GetCurrentWeapon())
		//{
		//	CurrentWeapon = Character->GetWeaponSystem()->GetCurrentWeapon();

		//	//SetAimSocket();
		//	UE_LOG(LogTemp, Warning, TEXT("Weapon Updated!!!"));
		//}

		//if (Character->GetWeaponSystem()->IsZoomIn())
		//{
		//	SetAimSocket();
		//}
	}
}

void USuraPlayerAnimInstance_Weapon::SetAimSocket()
{
	//TODO: Logic 손봐야함
	//if (IsValid(Character->GetWeaponSystem())
	//	&& Character->GetWeaponSystem() != nullptr)
	//{
	//	if (Character->GetWeaponSystem()->GetWeaponNum() != 0
	//		&& IsValid(Character->GetWeaponSystem()->GetCurrentWeapon())
	//		&& Character->GetWeaponSystem()->GetCurrentWeapon() != nullptr)
	//	{
	//		FTransform AimSocketTransform = Character->GetWeaponSystem()->GetCurrentWeapon()->GetSocketTransform(FName(TEXT("Aim")));
	//		FTransform RightHandTransform = Character->GetMesh()->GetSocketTransform(FName(TEXT("ik_hand_gun")));

	//		AimSocketRelativeTransform = AimSocketTransform.GetRelativeTransform(RightHandTransform);
	//	}
	//}
	//else
	//{
	//	FTransform AimSocketTransform = Character->GetMesh()->GetSocketTransform(FName(TEXT("ik_hand_gun")));
	//	FTransform RightHandTransform = Character->GetMesh()->GetSocketTransform(FName(TEXT("ik_hand_gun")));

	//	AimSocketRelativeTransform = AimSocketTransform.GetRelativeTransform(RightHandTransform);
	//}

	if (IsValid(SuraPlayer->GetWeaponSystemComponent())
		&& SuraPlayer->GetWeaponSystemComponent() != nullptr)
	{
		if (SuraPlayer->GetWeaponSystemComponent()->GetWeaponNum() != 0
			&& IsValid(SuraPlayer->GetWeaponSystemComponent()->GetCurrentWeapon())
			&& SuraPlayer->GetWeaponSystemComponent()->GetCurrentWeapon() != nullptr)
		{
			FTransform AimSocketTransform = SuraPlayer->GetWeaponSystemComponent()->GetCurrentWeapon()->GetWeaponMesh()->GetSocketTransform(FName(TEXT("Aim")));
			FTransform IKHandGunTransform = SuraPlayer->GetArmMesh()->GetSocketTransform(FName(TEXT("ik_hand_gun")));

			AimSocketRelativeTransform = AimSocketTransform.GetRelativeTransform(IKHandGunTransform);
			//AimSocketRelativeTransform = IKHandGunTransform.GetRelativeTransform(AimSocketTransform);
		}
	}
	else
	{
		FTransform AimSocketTransform = SuraPlayer->GetArmMesh()->GetSocketTransform(FName(TEXT("ik_hand_gun")));
		FTransform IKHandGunTransform = SuraPlayer->GetArmMesh()->GetSocketTransform(FName(TEXT("ik_hand_gun")));

		//AimSocketRelativeTransform = AimSocketTransform.GetRelativeTransform(IKHandGunTransform);
		AimSocketRelativeTransform = IKHandGunTransform.GetRelativeTransform(AimSocketTransform);
	}
}

void USuraPlayerAnimInstance_Weapon::SetAimPoint()
{
	FTransform CameraTransform = SuraPlayer->GetCameraComponent()->GetComponentTransform();
	//FTransform HandRootTransform = Character->GetMesh()->GetSocketTransform(FName(TEXT("ik_hand_root")));
	//FTransform HandRootTransform = Character->GetMesh()->GetBoneTransform(FName(TEXT("ik_hand_root")), ERelativeTransformSpace::RTS_World);
	FTransform HandRootTransform = SuraPlayer->GetArmMesh()->GetComponentTransform();

	FTransform CameraRelativeTransform = CameraTransform.GetRelativeTransform(HandRootTransform);
	//FTransform CameraRelativeTransform = HandRootTransform.GetRelativeTransform(CameraTransform);

	float CamOffset = 20.f; //TODO: 임시로 설정했음. 설정가능한 변수로 바꾸기

	AimPointLocation = CameraRelativeTransform.GetLocation() + CameraRelativeTransform.GetRotation().GetForwardVector() * CamOffset;
	AimPointRotation = CameraRelativeTransform.GetRotation().Rotator();
}

void USuraPlayerAnimInstance_Weapon::SetTargetRightHandTransform()
{
	if (IsValid(SuraPlayer->GetWeaponSystemComponent())
		&& SuraPlayer->GetWeaponSystemComponent() != nullptr)
	{
		if (SuraPlayer->GetWeaponSystemComponent()->GetWeaponNum() != 0
			&& IsValid(SuraPlayer->GetWeaponSystemComponent()->GetCurrentWeapon())
			&& SuraPlayer->GetWeaponSystemComponent()->GetCurrentWeapon() != nullptr)
		{
			//FTransform RootTransform = Character->GetMesh()->GetBoneTransform(FName(TEXT("ik_hand_root")), ERelativeTransformSpace::RTS_World);
			FTransform RootTransform = SuraPlayer->GetArmMesh()->GetComponentTransform();

			FTransform CameraTransform = SuraPlayer->GetCameraComponent()->GetComponentTransform();
			FTransform WeaponAimSocketTransform = SuraPlayer->GetWeaponSystemComponent()->GetCurrentWeapon()->GetWeaponMesh()->GetSocketTransform(FName(TEXT("Aim")));
			FTransform RightHandTransform = SuraPlayer->GetArmMesh()->GetBoneTransform(FName(TEXT("hand_r")));

			FTransform CameraRelativeTransform = CameraTransform.GetRelativeTransform(RootTransform);
			FTransform WeaponAimSocketRelativeTransform = WeaponAimSocketTransform.GetRelativeTransform(RootTransform);
			FTransform RightHandRelativeTransform = RightHandTransform.GetRelativeTransform(RootTransform);

			float CamOffset = 10.f; //TODO: 임시로 설정했음. 설정가능한 변수로 바꾸기

			AimPointRelativeLocation = CameraRelativeTransform.GetLocation() + CameraRelativeTransform.GetRotation().GetForwardVector() * CamOffset;
			AimPointRelativeRotation = CameraRelativeTransform.GetRotation().Rotator();

			HandTargetRelativeLocation = AimPointRelativeLocation + (RightHandRelativeTransform.GetLocation() - WeaponAimSocketRelativeTransform.GetLocation());
			HandTargetRelativeRotation = AimPointRelativeRotation + (RightHandRelativeTransform.GetRotation().Rotator() - WeaponAimSocketRelativeTransform.GetRotation().Rotator());
		}
	}
}


void USuraPlayerAnimInstance_Weapon::LogTransform(const FTransform& Transform, const FString& TransformName)
{
	FVector Location = Transform.GetLocation();
	FRotator Rotation = Transform.Rotator();
	FVector Scale = Transform.GetScale3D();

	UE_LOG(LogTemp, Log, TEXT("%s: Location: X=%.2f, Y=%.2f, Z=%.2f | Rotation: Pitch=%.2f, Yaw=%.2f, Roll=%.2f | Scale: X=%.2f, Y=%.2f, Z=%.2f"),
		*TransformName,
		Location.X, Location.Y, Location.Z,
		Rotation.Pitch, Rotation.Yaw, Rotation.Roll,
		Scale.X, Scale.Y, Scale.Z);
}

FTransform USuraPlayerAnimInstance_Weapon::GetLeftHandTransform()
{
	return SuraPlayer->GetArmMesh()->GetSocketTransform(FName("hand_l"));
}

FTransform USuraPlayerAnimInstance_Weapon::GetTargetLeftHandTransfrom()
{
	if (SuraPlayer)
	{
		CurrentWeapon = SuraPlayer->GetWeaponSystemComponent()->GetCurrentWeapon();
		if (CurrentWeapon)
		{
			return CurrentWeapon->GetWeaponMesh()->GetSocketTransform(FName("LeftHand"));
		}
	}

	//UE_LOG(LogTemp, Warning, TEXT("NOOOOOO!!!"));
	return FTransform();
}

#pragma region ArmRecoil
void USuraPlayerAnimInstance_Weapon::AddArmRecoil(float AdditionalRecoilAmountX, float AdditionalRecoilAmountY, float AdditionalRecoilAmountZ)
{
	// <Old Version>

	//bIsArmRecoiling = true;

	//if (&ArmRecoil_Hand && &ArmRecoil_UpperArm && &ArmRecoil_LowerArm)
	//{
	//	//UE_LOG(LogTemp, Warning, TEXT("Arm Recoil Added!!!"));

	//	// --- Hand ---
	//	FRotator RandRecoil_Hand;
	//	RandRecoil_Hand.Roll = FMath::RandRange((ArmRecoil_Hand.RecoilAmount.Roll + AdditionalRecoilAmountX) * ArmRecoil_Hand.RecoilRangeMin.Roll, (ArmRecoil_Hand.RecoilAmount.Roll + AdditionalRecoilAmountX) * ArmRecoil_Hand.RecoilRangeMax.Roll);
	//	RandRecoil_Hand.Pitch = FMath::RandRange((ArmRecoil_Hand.RecoilAmount.Pitch + AdditionalRecoilAmountY) * ArmRecoil_Hand.RecoilRangeMin.Pitch, (ArmRecoil_Hand.RecoilAmount.Pitch + AdditionalRecoilAmountY) * ArmRecoil_Hand.RecoilRangeMax.Pitch);
	//	RandRecoil_Hand.Yaw = FMath::RandRange((ArmRecoil_Hand.RecoilAmount.Yaw + AdditionalRecoilAmountZ) * ArmRecoil_Hand.RecoilRangeMin.Yaw, (ArmRecoil_Hand.RecoilAmount.Yaw + AdditionalRecoilAmountZ) * ArmRecoil_Hand.RecoilRangeMax.Yaw);

	//	TotalTargetRecoil_Hand += RandRecoil_Hand;

	//	// --- LowerArm ---
	//	FRotator RandRecoil_LowerArm;
	//	RandRecoil_LowerArm.Roll = FMath::RandRange((ArmRecoil_LowerArm.RecoilAmount.Roll + AdditionalRecoilAmountX) * ArmRecoil_LowerArm.RecoilRangeMin.Roll, (ArmRecoil_LowerArm.RecoilAmount.Roll + AdditionalRecoilAmountX) * ArmRecoil_LowerArm.RecoilRangeMax.Roll);
	//	RandRecoil_LowerArm.Pitch = FMath::RandRange((ArmRecoil_LowerArm.RecoilAmount.Pitch + AdditionalRecoilAmountY) * ArmRecoil_LowerArm.RecoilRangeMin.Pitch, (ArmRecoil_LowerArm.RecoilAmount.Pitch + AdditionalRecoilAmountY) * ArmRecoil_LowerArm.RecoilRangeMax.Pitch);
	//	RandRecoil_LowerArm.Yaw = FMath::RandRange((ArmRecoil_LowerArm.RecoilAmount.Yaw + AdditionalRecoilAmountZ) * ArmRecoil_LowerArm.RecoilRangeMin.Yaw, (ArmRecoil_LowerArm.RecoilAmount.Yaw + AdditionalRecoilAmountZ) * ArmRecoil_LowerArm.RecoilRangeMax.Yaw);

	//	TotalTargetRecoil_LowerArm += RandRecoil_LowerArm;

	//	// --- UpperArm ---
	//	FRotator RandRecoil_UpperArm;
	//	RandRecoil_UpperArm.Roll = FMath::RandRange((ArmRecoil_UpperArm.RecoilAmount.Roll + AdditionalRecoilAmountX) * ArmRecoil_UpperArm.RecoilRangeMin.Roll, (ArmRecoil_UpperArm.RecoilAmount.Roll + AdditionalRecoilAmountX) * ArmRecoil_UpperArm.RecoilRangeMax.Roll);
	//	RandRecoil_UpperArm.Pitch = FMath::RandRange((ArmRecoil_UpperArm.RecoilAmount.Pitch + AdditionalRecoilAmountY) * ArmRecoil_UpperArm.RecoilRangeMin.Pitch, (ArmRecoil_UpperArm.RecoilAmount.Pitch + AdditionalRecoilAmountY) * ArmRecoil_UpperArm.RecoilRangeMax.Pitch);
	//	RandRecoil_UpperArm.Yaw = FMath::RandRange((ArmRecoil_UpperArm.RecoilAmount.Yaw + AdditionalRecoilAmountZ) * ArmRecoil_UpperArm.RecoilRangeMin.Yaw, (ArmRecoil_UpperArm.RecoilAmount.Yaw + AdditionalRecoilAmountZ) * ArmRecoil_UpperArm.RecoilRangeMax.Yaw);

	//	TotalTargetRecoil_UpperArm += RandRecoil_UpperArm;
	//}

	//-----------------------------------------------
	// <New Version>

	bIsArmRecoiling = true;

	if (&ArmRecoil)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Arm Recoil Added!!!"));

		FRotator RandRecoil_Rot;
		FVector RandRecoil_Vec;

		RandRecoil_Rot.Roll = FMath::RandRange((ArmRecoil.Recoil_Rot.Roll + AdditionalRecoilAmountX) * ArmRecoil.RecoilRangeMin_Rot.Roll, (ArmRecoil.Recoil_Rot.Roll + AdditionalRecoilAmountX) * ArmRecoil.RecoilRangeMax_Rot.Roll);
		RandRecoil_Rot.Pitch = FMath::RandRange((ArmRecoil.Recoil_Rot.Pitch + AdditionalRecoilAmountY) * ArmRecoil.RecoilRangeMin_Rot.Pitch, (ArmRecoil.Recoil_Rot.Pitch + AdditionalRecoilAmountY) * ArmRecoil.RecoilRangeMax_Rot.Pitch);
		RandRecoil_Rot.Yaw = FMath::RandRange((ArmRecoil.Recoil_Rot.Yaw + AdditionalRecoilAmountZ) * ArmRecoil.RecoilRangeMin_Rot.Yaw, (ArmRecoil.Recoil_Rot.Yaw + AdditionalRecoilAmountZ) * ArmRecoil.RecoilRangeMax_Rot.Yaw);

		RandRecoil_Vec.X = FMath::RandRange((ArmRecoil.Recoil_Vec.X + AdditionalRecoilAmountX) * ArmRecoil.RecoilRangeMin_Vec.X, (ArmRecoil.Recoil_Vec.X + AdditionalRecoilAmountX) * ArmRecoil.RecoilRangeMax_Vec.X);
		RandRecoil_Vec.Y = FMath::RandRange((ArmRecoil.Recoil_Vec.Y + AdditionalRecoilAmountY) * ArmRecoil.RecoilRangeMin_Vec.Y, (ArmRecoil.Recoil_Vec.Y + AdditionalRecoilAmountY) * ArmRecoil.RecoilRangeMax_Vec.Y);
		RandRecoil_Vec.Z = FMath::RandRange((ArmRecoil.Recoil_Vec.Z + AdditionalRecoilAmountZ) * ArmRecoil.RecoilRangeMin_Vec.Z, (ArmRecoil.Recoil_Vec.Z + AdditionalRecoilAmountZ) * ArmRecoil.RecoilRangeMax_Vec.Z);

		TotalTargetRecoil_Rot += RandRecoil_Rot;
		TotalTargetRecoil_Vec += RandRecoil_Vec;
	}
}

void USuraPlayerAnimInstance_Weapon::ApplyArmRecoil(float DeltaTime)
{
	// <Old Version>

	//if (&ArmRecoil_Hand && &ArmRecoil_UpperArm && &ArmRecoil_LowerArm)
	//{
	//	// --- Hand ---
	//	FRotator InterpRecoilTarget_Hand = FMath::RInterpTo(FRotator(0.f, 0.f, 0.f), TotalTargetRecoil_Hand - CulmulatedRecoil_Hand, DeltaTime, ArmRecoil_Hand.RecoilSpeed);
	//	CurrentRecoil_Hand += InterpRecoilTarget_Hand;
	//	CulmulatedRecoil_Hand += InterpRecoilTarget_Hand;
	//	// --- LowerArm ---
	//	FRotator InterpRecoilTarget_LowerArm = FMath::RInterpTo(FRotator(0.f, 0.f, 0.f), TotalTargetRecoil_LowerArm - CulmulatedRecoil_LowerArm, DeltaTime, ArmRecoil_LowerArm.RecoilSpeed);
	//	CurrentRecoil_LowerArm += InterpRecoilTarget_LowerArm;
	//	CulmulatedRecoil_LowerArm += InterpRecoilTarget_LowerArm;
	//	// --- UpperArm ---
	//	FRotator InterpRecoilTarget_UpperArm = FMath::RInterpTo(FRotator(0.f, 0.f, 0.f), TotalTargetRecoil_UpperArm - CulmulatedRecoil_UpperArm, DeltaTime, ArmRecoil_UpperArm.RecoilSpeed);
	//	CurrentRecoil_UpperArm += InterpRecoilTarget_UpperArm;
	//	CulmulatedRecoil_UpperArm += InterpRecoilTarget_UpperArm;
	//}

	//-----------------------------------------------
	// <New Version>

	if (&ArmRecoil)
	{
		FRotator InterpRecoilTarget_Rot = FMath::RInterpTo(CurrentRecoil_Rot, TotalTargetRecoil_Rot, DeltaTime, ArmRecoil.RecoilSpeed);
		CurrentRecoil_Rot = InterpRecoilTarget_Rot;

		FVector InterpRecoilTarget_Vec = FMath::VInterpTo(CurrentRecoil_Vec, TotalTargetRecoil_Vec, DeltaTime, ArmRecoil.RecoilSpeed);
		CurrentRecoil_Vec = InterpRecoilTarget_Vec;
	}
}

void USuraPlayerAnimInstance_Weapon::RecoverArmRecoil(float DeltaTime)
{
	// <Old Version>

	//// --- Hand ---
	//FRotator InterpRecoilRecoverTarget_Hand = FMath::RInterpTo(FRotator(0.f, 0.f, 0.f), CulmulatedRecoil_Hand - RecoveredRecoil_Hand, DeltaTime, ArmRecoil_Hand.RecoilRecoverSpeed);
	//CurrentRecoil_Hand -= InterpRecoilRecoverTarget_Hand;
	//RecoveredRecoil_Hand += InterpRecoilRecoverTarget_Hand;

	//// --- LowerArm ---
	//FRotator InterpRecoilRecoverTarget_LowerArm = FMath::RInterpTo(FRotator(0.f, 0.f, 0.f), CulmulatedRecoil_LowerArm - RecoveredRecoil_LowerArm, DeltaTime, ArmRecoil_LowerArm.RecoilRecoverSpeed);
	//CurrentRecoil_LowerArm -= InterpRecoilRecoverTarget_LowerArm;
	//RecoveredRecoil_LowerArm += InterpRecoilRecoverTarget_LowerArm;

	//// --- UpperArm ---
	//FRotator InterpRecoilRecoverTarget_UpperArm = FMath::RInterpTo(FRotator(0.f, 0.f, 0.f), CulmulatedRecoil_UpperArm - RecoveredRecoil_UpperArm, DeltaTime, ArmRecoil_UpperArm.RecoilRecoverSpeed);
	//CurrentRecoil_UpperArm -= InterpRecoilRecoverTarget_UpperArm;
	//RecoveredRecoil_UpperArm += InterpRecoilRecoverTarget_UpperArm;

	//if ((CulmulatedRecoil_Hand - RecoveredRecoil_Hand).IsNearlyZero()
	//	&& (CulmulatedRecoil_LowerArm - RecoveredRecoil_LowerArm).IsNearlyZero()
	//	&& (CulmulatedRecoil_UpperArm - RecoveredRecoil_UpperArm).IsNearlyZero())
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Arm Recoil has been perfectly Recovered!!!"));

	//	TotalTargetRecoil_Hand = FRotator::ZeroRotator;
	//	TotalTargetRecoil_LowerArm = FRotator::ZeroRotator;
	//	TotalTargetRecoil_UpperArm = FRotator::ZeroRotator;

	//	CulmulatedRecoil_Hand = FRotator::ZeroRotator;
	//	CulmulatedRecoil_LowerArm = FRotator::ZeroRotator;
	//	CulmulatedRecoil_UpperArm = FRotator::ZeroRotator;

	//	RecoveredRecoil_Hand = FRotator::ZeroRotator;
	//	RecoveredRecoil_LowerArm = FRotator::ZeroRotator;
	//	RecoveredRecoil_UpperArm = FRotator::ZeroRotator;

	//	bIsArmRecoiling = false;
	//}

	//-----------------------------------------------
	// <New Version>
	FRotator InterpRecoilRecoverTarget_Rot = FMath::RInterpTo(TotalTargetRecoil_Rot, FRotator::ZeroRotator, DeltaTime, ArmRecoil.RecoilRecoverSpeed);
	TotalTargetRecoil_Rot = InterpRecoilRecoverTarget_Rot;

	FVector InterpRecoilRecoverTarget_Vec = FMath::VInterpTo(TotalTargetRecoil_Vec, FVector::ZeroVector, DeltaTime, ArmRecoil.RecoilRecoverSpeed);
	TotalTargetRecoil_Vec = InterpRecoilRecoverTarget_Vec;

	if (CurrentRecoil_Rot.IsNearlyZero()
		&& CurrentRecoil_Vec.IsNearlyZero())
	{
		UE_LOG(LogTemp, Warning, TEXT("Arm Recoil has been perfectly Recovered!!!"));

		TotalTargetRecoil_Rot = FRotator::ZeroRotator;
		CurrentRecoil_Rot = FRotator::ZeroRotator;

		TotalTargetRecoil_Vec = FVector::ZeroVector;
		CurrentRecoil_Vec = FVector::ZeroVector;

		bIsArmRecoiling = false;
	}
}

void USuraPlayerAnimInstance_Weapon::UpdateArmRecoil(float DeltaTime)
{
	if (bIsArmRecoiling)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Updating Arm Recoil"));

		ApplyArmRecoil(DeltaTime);
		RecoverArmRecoil(DeltaTime);
	}
}

void USuraPlayerAnimInstance_Weapon::ConvertRecoilValueFrame()
{
	if (bIsCrouching)
	{
		ConvertedCurrentRecoil_Rot = (RightHandSocketTransform_Crouch.GetRotation() * CurrentRecoil_Rot.Quaternion()).Rotator();
		ConvertedCurrentRecoil_Vec = RightHandSocketTransform_Crouch.GetRotation().RotateVector(CurrentRecoil_Vec) + RightHandSocketTransform_Crouch.GetTranslation();
	}
	else
	{
		ConvertedCurrentRecoil_Rot = (RightHandSocketTransform.GetRotation() * CurrentRecoil_Rot.Quaternion()).Rotator();
		ConvertedCurrentRecoil_Vec = RightHandSocketTransform.GetRotation().RotateVector(CurrentRecoil_Vec) + RightHandSocketTransform.GetTranslation();
	}
}
#pragma endregion