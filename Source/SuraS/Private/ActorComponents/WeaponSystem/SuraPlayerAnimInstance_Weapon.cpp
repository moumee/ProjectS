
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

	UE_LOG(LogTemp, Warning, TEXT("NOOOOOO!!!"));
	return FTransform();
}
