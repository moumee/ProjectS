

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Characters/PawnBasePlayer/SuraPlayerMovementComponent.h"
#include "ActorComponents/WeaponSystem/WeaponStateType.h"
#include "ActorComponents/WeaponSystem/WeaponInterface.h"
#include "ActorComponents/WeaponSystem/ArmRecoilStruct.h"
#include "PlayerAnimationData.h"
#include "SuraPlayerAnimInstance_Weapon.generated.h"

//class USuraPlayerBaseState;
class ASuraPawnPlayer;
class AWeapon;
/**
 * 
 */
UCLASS()
class SURAS_API USuraPlayerAnimInstance_Weapon : public UAnimInstance, public IWeaponInterface
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

protected:
	// <Player Movement>
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<ASuraPawnPlayer> SuraPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EMovementState MovementState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsRunning;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsCrouching;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsDashing;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector2D MovementInputVector;

	//-------------------------------------------------
	// <Weapon system>

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	FDataTableRowHandle AnimationDataTableHandle;
	FPlayerAnimationData* AnimationData;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	EWeaponStateType CurrentWeaponStateType;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	bool bIsZoomIn;

	//TODO: 사용안할 듯. 삭제요망
	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	FVector ScreenCenterWorldLocation;

	//TODO: 사용안할 듯. 삭제요망
	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	FVector TargetRightHandWorldLocation;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	FTransform AimSocketRelativeTransform;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	FTransform AimPointRelativeTransform;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	FTransform LeftHandTransform;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	FTransform TargetLeftHandSocketTransform;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	FVector AimPointLocation;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	FRotator AimPointRotation;

	//TODO: Logic 바꿔야 함
	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	AWeapon* CurrentWeapon;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	bool bHasWeapon = false;

	//--------------------------

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	FVector AimPointRelativeLocation;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	FRotator AimPointRelativeRotation;

	//---------------------------------------

	//UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	//FTransform DefalutCameraRelativeTransform;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	FVector HandTargetRelativeLocation;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	FRotator HandTargetRelativeRotation;

public:
	void LoadAnimationData();

	void UpdateWeapon();

	void SetAimSocket();

	void SetAimPoint();

	//---------------------------

	void SetTargetRightHandTransform();

	void LogTransform(const FTransform& Transform, const FString& TransformName = TEXT("Transform"));

	//---------------------------
	FTransform GetLeftHandTransform();
	FTransform GetTargetLeftHandTransfrom();

#pragma region RightHandSocket
protected:
	UPROPERTY(BlueprintReadOnly, Category = "RightHandSocket")
	FTransform RightHandSocketTransform;
	UPROPERTY(BlueprintReadOnly, Category = "RightHandSocket")
	FTransform RightHandSocketTransform_Crouch;

	UPROPERTY(BlueprintReadOnly, Category = "RightHandSocket")
	FTransform CurrentRightHandSocketTransform;
public:
	void UpdateRightHandSocket(float DeltaTime);
#pragma endregion

#pragma region ArmRecoil
public:
	virtual void AddArmRecoil(float AdditionalRecoilAmountX = 0.f, float AdditionalRecoilAmountY = 0.f, float AdditionalRecoilAmountZ = 0.f) override;
protected:
	bool bIsArmRecoiling = false;

	// <Old>
	UPROPERTY()
	FArmRecoilStruct ArmRecoil_Hand;
	UPROPERTY()
	FArmRecoilStruct ArmRecoil_UpperArm;
	UPROPERTY()
	FArmRecoilStruct ArmRecoil_LowerArm;

	FRotator TotalTargetRecoil_Hand = { 0.f, 0.f, 0.f };
	FRotator TotalTargetRecoil_LowerArm = { 0.f, 0.f, 0.f };
	FRotator TotalTargetRecoil_UpperArm = { 0.f, 0.f, 0.f };

	FRotator CulmulatedRecoil_Hand = { 0.f, 0.f, 0.f };
	FRotator CulmulatedRecoil_LowerArm = { 0.f, 0.f, 0.f };
	FRotator CulmulatedRecoil_UpperArm = { 0.f, 0.f, 0.f };

	FRotator RecoveredRecoil_Hand = { 0.f, 0.f, 0.f };
	FRotator RecoveredRecoil_LowerArm = { 0.f, 0.f, 0.f };
	FRotator RecoveredRecoil_UpperArm = { 0.f, 0.f, 0.f };

	UPROPERTY(BlueprintReadOnly, Category = "ArmRecoil")
	FRotator CurrentRecoil_Hand = { 0.f, 0.f, 0.f };
	UPROPERTY(BlueprintReadOnly, Category = "ArmRecoil")
	FRotator CurrentRecoil_LowerArm = { 0.f, 0.f, 0.f };
	UPROPERTY(BlueprintReadOnly, Category = "ArmRecoil")
	FRotator CurrentRecoil_UpperArm = { 0.f, 0.f, 0.f };

	//--------------------------
	// <New>
	UPROPERTY()
	FArmRecoilStruct ArmRecoil;

	FRotator TotalTargetRecoil_Rot;
	FVector TotalTargetRecoil_Vec;	

	UPROPERTY(BlueprintReadOnly, Category = "ArmRecoil")
	FRotator CurrentRecoil_Rot;
	UPROPERTY(BlueprintReadOnly, Category = "ArmRecoil")
	FVector CurrentRecoil_Vec;

	UPROPERTY(BlueprintReadOnly, Category = "ArmRecoil")
	FRotator ConvertedCurrentRecoil_Rot;
	UPROPERTY(BlueprintReadOnly, Category = "ArmRecoil")
	FVector ConvertedCurrentRecoil_Vec;

public:
	void ApplyArmRecoil(float DeltaTime);
	void RecoverArmRecoil(float DeltaTime);
	void UpdateArmRecoil(float DeltaTime);
	void ConvertRecoilValueFrame();
#pragma endregion

#pragma region Crouch
protected:
	FRotator RotationWhenCrouching_Hand_R;
	FRotator RotationWhenCrouching_LowerArm_R;
	FRotator RotationWhenCrouching_UpperArm_R;
#pragma endregion

#pragma region Spring Damper
protected:

	FVector PrevComponentPos;
	FVector PrevComponentVel;

	FVector CurrentComponentPos;
	FVector CurrentComponentVel;

	FTransform RightHandSocketSpringDamperTransform;

protected:
	FVector Stiffness;
	FVector Damping;

	float fast_negexp(float x);
	//void SpringDamper(FVector CurrPos, FVector CurrVel, FVector GoalPos, FVector GoalVel, FVector& OutPos, FVector& OutVel, float stiffness, float damping, float DeltaTime, float eps = 1e-5f);
	
	void SpringDamepr_2(FVector CurrPos, FVector CurrVel, FVector GoalPos, FVector GoalVel, FVector& OutPos, FVector& OutVel, FVector stiffness, FVector damping, float DeltaTime, float eps = 1e-5f);
	
	void UpdateSpringDamper(float DeltaTime);

	void UpdateSpringDamepr_Test(float DeltaTime);

#pragma endregion

};
