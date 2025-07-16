
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
		LoadAnimationData();

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
			UpdateRightHandSocket(DeltaTime);
			SetAimPoint();
			UpdateWeapon();

			UpdateSpringDamper(DeltaTime);

			UpdateArmRecoil(DeltaTime);
			ConvertRecoilValueFrame();

			LeftHandTransform = GetLeftHandTransform();
			TargetLeftHandSocketTransform = GetTargetLeftHandTransfrom();
		}

		// DefalutCameraRelativeTransform = Character->GetCamera()->GetRelativeTransform();
		// LogTransform(AimSocketRelativeTransform);
	}
}

void USuraPlayerAnimInstance_Weapon::LoadAnimationData()
{
	AnimationData = AnimationDataTableHandle.GetRow<FPlayerAnimationData>("");
	if (AnimationData)
	{
		DamperScale = AnimationData->DamperScale;
		Stiffness = AnimationData->Stiffness;
		Damping = AnimationData->Damping;
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

#pragma region RightHandSocket
void USuraPlayerAnimInstance_Weapon::UpdateRightHandSocket(float DeltaTime)
{
	if (bIsCrouching)
	{
		CurrentRightHandSocketTransform.SetRotation((FMath::RInterpTo(CurrentRightHandSocketTransform.GetRotation().Rotator(), RightHandSocketTransform_Crouch.GetRotation().Rotator(), DeltaTime, 10.f)).Quaternion());
		CurrentRightHandSocketTransform.SetTranslation(FMath::VInterpTo(CurrentRightHandSocketTransform.GetTranslation(), RightHandSocketTransform_Crouch.GetTranslation(), DeltaTime, 10.f));
	}
	else
	{
		CurrentRightHandSocketTransform.SetRotation((FMath::RInterpTo(CurrentRightHandSocketTransform.GetRotation().Rotator(), RightHandSocketTransform.GetRotation().Rotator(), DeltaTime, 10.f)).Quaternion());
		CurrentRightHandSocketTransform.SetTranslation(FMath::VInterpTo(CurrentRightHandSocketTransform.GetTranslation(), RightHandSocketTransform.GetTranslation(), DeltaTime, 10.f));
	}
}
#pragma endregion

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
	// <Old Version>
	//if (bIsCrouching)
	//{
	//	ConvertedCurrentRecoil_Rot = (RightHandSocketTransform_Crouch.GetRotation() * CurrentRecoil_Rot.Quaternion()).Rotator();
	//	ConvertedCurrentRecoil_Vec = RightHandSocketTransform_Crouch.GetRotation().RotateVector(CurrentRecoil_Vec) + RightHandSocketTransform_Crouch.GetTranslation();
	//}
	//else
	//{
	//	ConvertedCurrentRecoil_Rot = (RightHandSocketTransform.GetRotation() * CurrentRecoil_Rot.Quaternion()).Rotator();
	//	ConvertedCurrentRecoil_Vec = RightHandSocketTransform.GetRotation().RotateVector(CurrentRecoil_Vec) + RightHandSocketTransform.GetTranslation();
	//}
	//----------------------
	// <New Version>
	//ConvertedCurrentRecoil_Rot = (CurrentRightHandSocketTransform.GetRotation() * CurrentRecoil_Rot.Quaternion()).Rotator();
	//ConvertedCurrentRecoil_Vec = CurrentRightHandSocketTransform.GetRotation().RotateVector(CurrentRecoil_Vec) + CurrentRightHandSocketTransform.GetTranslation();

	// <New New Version>
	ConvertedCurrentRecoil_Rot = (RightHandSocketSpringDamperTransform.GetRotation() * CurrentRecoil_Rot.Quaternion()).Rotator();
	ConvertedCurrentRecoil_Vec = RightHandSocketSpringDamperTransform.GetRotation().RotateVector(CurrentRecoil_Vec) + RightHandSocketSpringDamperTransform.GetTranslation();
}
#pragma endregion

#pragma region Spring Damper
float USuraPlayerAnimInstance_Weapon::fast_negexp(float x)
{
	return 1.0f / (1.0f + x + 0.48f * x * x + 0.235f * x * x * x);
}
void USuraPlayerAnimInstance_Weapon::SpringDamper(FVector CurrPos, FVector CurrVel, FVector GoalPos, FVector GoalVel, FVector& OutPos, FVector& OutVel, FVector stiffness, FVector damping, float DeltaTime, float eps)
{
	FVector x = CurrPos;
	FVector v = CurrVel;

	FVector g = GoalPos;
	FVector q = GoalVel;
	FVector s = stiffness;
	FVector d = damping;
	FVector c = g + (d * q) / (s + eps);
	FVector y = d / 2.0f;

	// <X>
	if (fabs(s.X - (d.X * d.X) / 4.0f) < eps) // Critically Damped
	{
		float j0 = x.X - c.X;
		float j1 = v.X + j0 * y.X;

		float eydt = fast_negexp(y.X * DeltaTime);

		x.X = j0 * eydt + DeltaTime * j1 * eydt + c.X;
		v.X = -y.X * j0 * eydt - y.X * DeltaTime * j1 * eydt + j1 * eydt;
	}
	else if (s.X - (d.X * d.X) / 4.0f > 0.0) // Under Damped
	{
		float w = sqrtf(s.X - (d.X * d.X) / 4.0f);
		float j = sqrtf(FMath::Square(v.X + y.X * (x.X - c.X)) / (w * w + eps) + FMath::Square(x.X - c.X));
		float p = FMath::Atan((v.X + (x.X - c.X) * y.X) / (-(x.X - c.X) * w + eps));

		j = (x.X - c.X) > 0.0f ? j : -j;

		float eydt = fast_negexp(y.X * DeltaTime);

		x.X = j * eydt * cosf(w * DeltaTime + p) + c.X;
		v.X = -y.X * j * eydt * cosf(w * DeltaTime + p) - w * j * eydt * sinf(w * DeltaTime + p);
	}
	else if (s.X - (d.X * d.X) / 4.0f < 0.0) // Over Damped
	{
		float y0 = (d.X + sqrtf(d.X * d.X - 4 * s.X)) / 2.0f;
		float y1 = (d.X - sqrtf(d.X * d.X - 4 * s.X)) / 2.0f;

		float j1 = (c.X * y0 - x.X * y0 - v.X) / (y1 - y0);
		float j0 = x.X - j1 - c.X;

		float ey0dt = fast_negexp(y0 * DeltaTime);
		float ey1dt = fast_negexp(y1 * DeltaTime);

		x.X = j0 * ey0dt + j1 * ey1dt + c.X;
		v.X = -y0 * j0 * ey0dt - y1 * j1 * ey1dt;
	}

	// <Y>
	if (fabs(s.Y - (d.Y * d.Y) / 4.0f) < eps) // Critically Damped
	{
		float j0 = x.Y - c.Y;
		float j1 = v.Y + j0 * y.Y;

		float eydt = fast_negexp(y.Y * DeltaTime);

		x.Y = j0 * eydt + DeltaTime * j1 * eydt + c.Y;
		v.Y = -y.Y * j0 * eydt - y.Y * DeltaTime * j1 * eydt + j1 * eydt;
	}
	else if (s.Y - (d.Y * d.Y) / 4.0f > 0.0) // Under Damped
	{
		float w = sqrtf(s.Y - (d.Y * d.Y) / 4.0f);
		float j = sqrtf(FMath::Square(v.Y + y.Y * (x.Y - c.Y)) / (w * w + eps) + FMath::Square(x.Y - c.Y));
		float p = FMath::Atan((v.Y + (x.Y - c.Y) * y.Y) / (-(x.Y - c.Y) * w + eps));

		j = (x.Y - c.Y) > 0.0f ? j : -j;

		float eydt = fast_negexp(y.Y * DeltaTime);

		x.Y = j * eydt * cosf(w * DeltaTime + p) + c.Y;
		v.Y = -y.Y * j * eydt * cosf(w * DeltaTime + p) - w * j * eydt * sinf(w * DeltaTime + p);
	}
	else if (s.Y - (d.Y * d.Y) / 4.0f < 0.0) // Over Damped
	{
		float y0 = (d.Y + sqrtf(d.Y * d.Y - 4 * s.Y)) / 2.0f;
		float y1 = (d.Y - sqrtf(d.Y * d.Y - 4 * s.Y)) / 2.0f;

		float j1 = (c.Y * y0 - x.Y * y0 - v.Y) / (y1 - y0);
		float j0 = x.Y - j1 - c.Y;

		float ey0dt = fast_negexp(y0 * DeltaTime);
		float ey1dt = fast_negexp(y1 * DeltaTime);

		x.Y = j0 * ey0dt + j1 * ey1dt + c.Y;
		v.Y = -y0 * j0 * ey0dt - y1 * j1 * ey1dt;
	}

	// <Z>
	if (fabs(s.Z - (d.Z * d.Z) / 4.0f) < eps) // Critically Damped
	{
		float j0 = x.Z - c.Z;
		float j1 = v.Z + j0 * y.Z;

		float eydt = fast_negexp(y.Z * DeltaTime);

		x.Z = j0 * eydt + DeltaTime * j1 * eydt + c.Z;
		v.Z = -y.Z * j0 * eydt - y.Z * DeltaTime * j1 * eydt + j1 * eydt;
	}
	else if (s.Z - (d.Z * d.Z) / 4.0f > 0.0) // Under Damped
	{
		float w = sqrtf(s.Z - (d.Z * d.Z) / 4.0f);
		float j = sqrtf(FMath::Square(v.Z + y.Z * (x.Z - c.Z)) / (w * w + eps) + FMath::Square(x.Z - c.Z));
		float p = FMath::Atan((v.Z + (x.Z - c.Z) * y.Z) / (-(x.Z - c.Z) * w + eps));

		j = (x.Z - c.Z) > 0.0f ? j : -j;

		float eydt = fast_negexp(y.Z * DeltaTime);

		x.Z = j * eydt * cosf(w * DeltaTime + p) + c.Z;
		v.Z = -y.Z * j * eydt * cosf(w * DeltaTime + p) - w * j * eydt * sinf(w * DeltaTime + p);
	}
	else if (s.Z - (d.Z * d.Z) / 4.0f < 0.0) // Over Damped
	{
		float y0 = (d.Z + sqrtf(d.Z * d.Z - 4 * s.Z)) / 2.0f;
		float y1 = (d.Z - sqrtf(d.Z * d.Z - 4 * s.Z)) / 2.0f;

		float j1 = (c.Z * y0 - x.Z * y0 - v.Z) / (y1 - y0);
		float j0 = x.Z - j1 - c.Z;

		float ey0dt = fast_negexp(y0 * DeltaTime);
		float ey1dt = fast_negexp(y1 * DeltaTime);

		x.Z = j0 * ey0dt + j1 * ey1dt + c.Z;
		v.Z = -y0 * j0 * ey0dt - y1 * j1 * ey1dt;
	}

	OutPos = x;
	OutVel = v;
}
void USuraPlayerAnimInstance_Weapon::UpdateSpringDamper(float DeltaTime)
{
	FTransform MeshToWorldTransform = SuraPlayer->GetArmMesh()->GetComponentTransform();
	FTransform ActorToWorldTransform = SuraPlayer->GetTransform();

	FVector CurrPos = ActorToWorldTransform.InverseTransformPosition(CurrentComponentPos); //Actor
	FVector CurrVel = ActorToWorldTransform.InverseTransformVector(CurrentComponentVel); //Actor

	FVector GoalPos = ActorToWorldTransform.InverseTransformPosition(MeshToWorldTransform.TransformPosition(CurrentRightHandSocketTransform.GetLocation())); //Actor
	FVector GoalVel = ActorToWorldTransform.InverseTransformVector(SuraPlayer->GetVelocity()); //Actor
	//FVector GoalVel = ActorToWorldTransform.InverseTransformVector(Velocity); //Actor

	FVector OutPos; //Actor
	FVector OutVel; //Actor

	SpringDamper(CurrPos, CurrVel, GoalPos, GoalVel, OutPos, OutVel, Stiffness, Damping, DeltaTime); //Local 기준 계산

	//UE_LOG(LogTemp, Warning, TEXT("GoalVel: %s"), *GoalVel.ToString());
	//UE_LOG(LogTemp, Warning, TEXT("GoalPos: %s"), *GoalPos.ToString());
	//UE_LOG(LogTemp, Warning, TEXT("CurrVel: %s"), *CurrVel.ToString());
	//UE_LOG(LogTemp, Warning, TEXT("CurrPos: %s"), *CurrPos.ToString());

	//UE_LOG(LogTemp, Warning, TEXT("Velocity: %s"), *Velocity.ToString());
	//UE_LOG(LogTemp, Warning, TEXT("SuraPlayer->GetVelocity(): %s"), *SuraPlayer->GetVelocity().ToString());

	FVector ConvertedPos = (GoalPos + (OutPos - GoalPos) * DamperScale);
	ConvertedPos.X = GoalPos.X;

	ConvertedPos = MeshToWorldTransform.InverseTransformPosition(ActorToWorldTransform.TransformPosition(ConvertedPos));

	CurrentComponentPos = ActorToWorldTransform.TransformPosition(OutPos); //World
	CurrentComponentVel = ActorToWorldTransform.TransformVector(OutVel); //World

	FVector DirectionVec = ConvertedPos - (CurrentRightHandSocketTransform.GetLocation() + FVector(0.f, -20.f, 0.f));
	FQuat RotByZ = FQuat(FVector::ZAxisVector, -FMath::Atan(DirectionVec.X / DirectionVec.Y));
	FQuat RotByX = FQuat(FVector::XAxisVector, FMath::Atan(DirectionVec.Z / FMath::Sqrt(DirectionVec.X * DirectionVec.X + DirectionVec.Y * DirectionVec.Y)));

	RightHandSocketSpringDamperTransform.SetLocation(ConvertedPos);
	RightHandSocketSpringDamperTransform.SetRotation(RotByX * RotByZ * CurrentRightHandSocketTransform.GetRotation());
}
#pragma endregion