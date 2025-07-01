
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
		bIsCrouching = SuraPlayer->GetPlayerMovementComponent()->IsCrouching();
		bIsDashing = SuraPlayer->GetPlayerMovementComponent()->IsDashing();
		Velocity = SuraPlayer->GetPlayerMovementComponent()->Velocity;
		MovementInputVector = SuraPlayer->GetPlayerMovementComponent()->GetMovementInputVector();


		//-------------------------------------------
		// <weapon System>
		SetAimPoint();
		CurrentWeaponStateType = EWeaponStateType::WeaponStateType_None;
	}

	if (SuraPlayer && SuraPlayer->GetWeaponSystemComponent())
	{
		//SetAimSocket();
	}
}

void USuraPlayerAnimInstance_Weapon::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (SuraPlayer)
	{
		// <Character Movement>
		MovementState = SuraPlayer->GetPlayerMovementComponent()->GetMovementState();
		bIsCrouching = SuraPlayer->GetPlayerMovementComponent()->IsCrouching();
		bIsDashing = SuraPlayer->GetPlayerMovementComponent()->IsDashing();
		Velocity = SuraPlayer->GetPlayerMovementComponent()->Velocity;
		MovementInputVector = SuraPlayer->GetPlayerMovementComponent()->GetMovementInputVector();


		//-----------------------------------------------
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

			if (CurrentWeaponStateType == EWeaponStateType::WeaponStateType_Firing)
			{
				UE_LOG(LogTemp, Warning, TEXT("Firing State!!!!!!!!!!"));
			}
			else if (CurrentWeaponStateType == EWeaponStateType::WeaponStateType_Reloading)
			{
				UE_LOG(LogTemp, Warning, TEXT("Reloading State!!!!!!!!!!"));
			}

			//-------------------------------------------------------------

			if (CurrentWeapon != SuraPlayer->GetWeaponSystemComponent()->GetCurrentWeapon())
			{
				DefaultArmRecoil = *SuraPlayer->GetWeaponSystemComponent()->GetCurrentWeapon()->GetArmRecoilInfo();
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
	bIsArmRecoiling = true;

	if (&DefaultArmRecoil)
	{
		UE_LOG(LogTemp, Warning, TEXT("Arm Recoil Added!!!"));


		//float RandRecoilX = FMath::RandRange((DefaultArmRecoil->RecoilAmountX + AdditionalRecoilAmountX) * DefaultArmRecoil->RecoilRangeMinX, (DefaultArmRecoil->RecoilAmountX + AdditionalRecoilAmountX) * DefaultArmRecoil->RecoilRangeMaxX) * (-1);
		//float RandRecoilY = FMath::RandRange((DefaultArmRecoil->RecoilAmountY + AdditionalRecoilAmountY) * DefaultArmRecoil->RecoilRangeMinY, (DefaultArmRecoil->RecoilAmountY + AdditionalRecoilAmountY) * DefaultArmRecoil->RecoilRangeMaxY);
		//float RandRecoilZ = FMath::RandRange((DefaultArmRecoil->RecoilAmountZ + AdditionalRecoilAmountZ) * DefaultArmRecoil->RecoilRangeMinZ, (DefaultArmRecoil->RecoilAmountZ + AdditionalRecoilAmountZ) * DefaultArmRecoil->RecoilRangeMaxZ);

		float RandRecoilX = FMath::RandRange((DefaultArmRecoil.RecoilAmountX + AdditionalRecoilAmountX) * DefaultArmRecoil.RecoilRangeMinX, (DefaultArmRecoil.RecoilAmountX + AdditionalRecoilAmountX) * DefaultArmRecoil.RecoilRangeMaxX) * (-1);
		float RandRecoilY = FMath::RandRange((DefaultArmRecoil.RecoilAmountY + AdditionalRecoilAmountY) * DefaultArmRecoil.RecoilRangeMinY, (DefaultArmRecoil.RecoilAmountY + AdditionalRecoilAmountY) * DefaultArmRecoil.RecoilRangeMaxY);
		float RandRecoilZ = FMath::RandRange((DefaultArmRecoil.RecoilAmountZ + AdditionalRecoilAmountZ) * DefaultArmRecoil.RecoilRangeMinZ, (DefaultArmRecoil.RecoilAmountZ + AdditionalRecoilAmountZ) * DefaultArmRecoil.RecoilRangeMaxZ);

		TotalTargetArmRecoilValueX += RandRecoilX;
		TotalTargetArmRecoilValueY += RandRecoilY;
		TotalTargetArmRecoilValueZ += RandRecoilZ;
	}
}

void USuraPlayerAnimInstance_Weapon::ApplyArmRecoil(float DeltaTime, FArmRecoilStruct* RecoilStruct)
{
	//if (RecoilStruct)
	//{
	//	CurrentArmRecoilValueX = FMath::FInterpTo(CurrentArmRecoilValueX, TotalTargetArmRecoilValueX, DeltaTime, RecoilStruct->RecoilSpeed);
	//	CurrentArmRecoilValueY = FMath::FInterpTo(CurrentArmRecoilValueY, TotalTargetArmRecoilValueY, DeltaTime, RecoilStruct->RecoilSpeed);
	//	CurrentArmRecoilValueZ = FMath::FInterpTo(CurrentArmRecoilValueZ, TotalTargetArmRecoilValueZ, DeltaTime, RecoilStruct->RecoilSpeed);
	//}

	//--------------------------------------------------

	if (RecoilStruct)
	{
		float InterpRecoilTargetValue_X = FMath::FInterpTo(0.f, TotalTargetArmRecoilValueX - CulmulatedRecoilX, DeltaTime, RecoilStruct->RecoilSpeed);
		float InterpRecoilTargetValue_Y = FMath::FInterpTo(0.f, TotalTargetArmRecoilValueY - CulmulatedRecoilY, DeltaTime, RecoilStruct->RecoilSpeed);
		float InterpRecoilTargetValue_Z = FMath::FInterpTo(0.f, TotalTargetArmRecoilValueZ - CulmulatedRecoilZ, DeltaTime, RecoilStruct->RecoilSpeed);

		CurrentArmRecoilValueX += InterpRecoilTargetValue_X;
		CurrentArmRecoilValueY += InterpRecoilTargetValue_Y;
		CurrentArmRecoilValueZ += InterpRecoilTargetValue_Z;

		CulmulatedRecoilX += InterpRecoilTargetValue_X;
		CulmulatedRecoilY += InterpRecoilTargetValue_Y;
		CulmulatedRecoilZ += InterpRecoilTargetValue_Z;
	}
}

void USuraPlayerAnimInstance_Weapon::RecoverArmRecoil(float DeltaTime, FArmRecoilStruct* RecoilStruct)
{

	//CurrentArmRecoilValueX = FMath::FInterpTo(CurrentArmRecoilValueX, 0.f, DeltaTime, RecoilStruct->RecoilRecoverSpeed);
	//CurrentArmRecoilValueY = FMath::FInterpTo(CurrentArmRecoilValueY, 0.f, DeltaTime, RecoilStruct->RecoilRecoverSpeed);
	//CurrentArmRecoilValueZ = FMath::FInterpTo(CurrentArmRecoilValueZ, 0.f, DeltaTime, RecoilStruct->RecoilRecoverSpeed);

	//if (FMath::Abs(CurrentArmRecoilValueX) < KINDA_SMALL_NUMBER
	//	&& FMath::Abs(CurrentArmRecoilValueX) < KINDA_SMALL_NUMBER
	//	&& FMath::Abs(CurrentArmRecoilValueZ) < KINDA_SMALL_NUMBER)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("Arm Recoil has been perfectly Recovered!!!"));

	//	TotalTargetArmRecoilValueX = 0.f;
	//	TotalTargetArmRecoilValueY = 0.f;
	//	TotalTargetArmRecoilValueZ = 0.f;

	//	CurrentArmRecoilValueX = 0.f;
	//	CurrentArmRecoilValueY = 0.f;
	//	CurrentArmRecoilValueZ = 0.f;

	//	bIsArmRecoiling = false;
	//}

	//------------------------------------------------

	float InterpRecoilRecoverTargetValue_X = FMath::FInterpTo(0.f, CulmulatedRecoilX - RecoveredArmRecoilX, DeltaTime, RecoilStruct->RecoilRecoverSpeed);
	float InterpRecoilRecoverTargetValue_Y = FMath::FInterpTo(0.f, CulmulatedRecoilY - RecoveredArmRecoilY, DeltaTime, RecoilStruct->RecoilRecoverSpeed);
	float InterpRecoilRecoverTargetValue_Z = FMath::FInterpTo(0.f, CulmulatedRecoilZ - RecoveredArmRecoilZ, DeltaTime, RecoilStruct->RecoilRecoverSpeed);

	CurrentArmRecoilValueX -= InterpRecoilRecoverTargetValue_X;
	CurrentArmRecoilValueY -= InterpRecoilRecoverTargetValue_Y;
	CurrentArmRecoilValueZ -= InterpRecoilRecoverTargetValue_Z;

	RecoveredArmRecoilX += InterpRecoilRecoverTargetValue_X;
	RecoveredArmRecoilY += InterpRecoilRecoverTargetValue_Y;
	RecoveredArmRecoilZ += InterpRecoilRecoverTargetValue_Z;

	if (FMath::Abs(CulmulatedRecoilX - RecoveredArmRecoilX) < KINDA_SMALL_NUMBER
		&& FMath::Abs(CulmulatedRecoilY - RecoveredArmRecoilY) < KINDA_SMALL_NUMBER
		&& FMath::Abs(CulmulatedRecoilZ - RecoveredArmRecoilZ) < KINDA_SMALL_NUMBER)
	{
		UE_LOG(LogTemp, Warning, TEXT("Arm Recoil has been perfectly Recovered!!!"));

		TotalTargetArmRecoilValueX = 0.f;
		TotalTargetArmRecoilValueY = 0.f;
		TotalTargetArmRecoilValueZ = 0.f;

		CulmulatedRecoilX = 0.f;
		CulmulatedRecoilY = 0.f;
		CulmulatedRecoilZ = 0.f;

		RecoveredArmRecoilX = 0.f;
		RecoveredArmRecoilY = 0.f;
		RecoveredArmRecoilZ = 0.f;

		bIsArmRecoiling = false;
	}
}

void USuraPlayerAnimInstance_Weapon::UpdateArmRecoil(float DeltaTime)
{
	if (bIsArmRecoiling)
	{
		//ApplyArmRecoil(DeltaTime, DefaultArmRecoil);
		//RecoverArmRecoil(DeltaTime, DefaultArmRecoil);

		UE_LOG(LogTemp, Warning, TEXT("Updating Arm Recoil"));

		ApplyArmRecoil(DeltaTime, &DefaultArmRecoil);
		RecoverArmRecoil(DeltaTime, &DefaultArmRecoil);
	}
}
#pragma endregion