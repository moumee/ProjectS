

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Characters/PawnBasePlayer/SuraPlayerMovementComponent.h"
#include "ActorComponents/WeaponSystem/WeaponStateType.h"
#include "ActorComponents/WeaponSystem/WeaponInterface.h"
#include "ActorComponents/WeaponSystem/ArmRecoilStruct.h"
#include "SuraPlayerAnimInstance_Weapon.generated.h"

class USuraPlayerBaseState;
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
	//SuraPlayerAnimInstance_Weapon();
	//~SuraPlayerAnimInstance_Weapon();

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

protected:
	// <Player Movement>
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<ASuraPawnPlayer> SuraPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EMovementState MovementState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsCrouching;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsDashing;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector2D MovementInputVector;


	//-------------------------------------------------
	// <이재형 Weapon system 관련>

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
	void UpdateWeapon();

	void SetAimSocket();

	void SetAimPoint();

	//---------------------------

	void SetTargetRightHandTransform();

	void LogTransform(const FTransform& Transform, const FString& TransformName = TEXT("Transform"));

	//---------------------------
	FTransform GetLeftHandTransform();
	FTransform GetTargetLeftHandTransfrom();

	
#pragma region ArmRecoil
public:
	virtual void AddArmRecoil(float AdditionalRecoilAmountX = 0.f, float AdditionalRecoilAmountY = 0.f, float AdditionalRecoilAmountZ = 0.f) override;

protected:
	bool bIsArmRecoiling = false;

	UPROPERTY()
	FArmRecoilStruct ArmRecoil_Hand;
	UPROPERTY()
	FArmRecoilStruct ArmRecoil_UpperArm;
	UPROPERTY()
	FArmRecoilStruct ArmRecoil_LowerArm;

	FVector TotalTargetRecoil_Hand = { 0.f, 0.f, 0.f };
	FVector TotalTargetRecoil_LowerArm = { 0.f, 0.f, 0.f };
	FVector TotalTargetRecoil_UpperArm = { 0.f, 0.f, 0.f };

	FVector CulmulatedRecoil_Hand = { 0.f, 0.f, 0.f };
	FVector CulmulatedRecoil_LowerArm = { 0.f, 0.f, 0.f };
	FVector CulmulatedRecoil_UpperArm = { 0.f, 0.f, 0.f };

	FVector RecoveredRecoil_Hand = { 0.f, 0.f, 0.f };
	FVector RecoveredRecoil_LowerArm = { 0.f, 0.f, 0.f };
	FVector RecoveredRecoil_UpperArm = { 0.f, 0.f, 0.f };

	//float TotalTargetArmRecoilValueX = 0.f;
	//float TotalTargetArmRecoilValueY = 0.f;
	//float TotalTargetArmRecoilValueZ = 0.f;

	//float CulmulatedRecoilX = 0.f;
	//float CulmulatedRecoilY = 0.f;
	//float CulmulatedRecoilZ = 0.f;

	//float RecoveredArmRecoilX = 0.f;
	//float RecoveredArmRecoilY = 0.f;
	//float RecoveredArmRecoilZ = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "ArmRecoil")
	FVector CurrentRecoil_Hand = { 0.f, 0.f, 0.f };
	UPROPERTY(BlueprintReadOnly, Category = "ArmRecoil")
	FVector CurrentRecoil_LowerArm = { 0.f, 0.f, 0.f };
	UPROPERTY(BlueprintReadOnly, Category = "ArmRecoil")
	FVector CurrentRecoil_UpperArm = { 0.f, 0.f, 0.f };

	//UPROPERTY(BlueprintReadOnly, Category = "ArmRecoil")
	//float CurrentArmRecoilValueX = 0.f;
	//UPROPERTY(BlueprintReadOnly, Category = "ArmRecoil")
	//float CurrentArmRecoilValueY = 0.f;
	//UPROPERTY(BlueprintReadOnly, Category = "ArmRecoil")
	//float CurrentArmRecoilValueZ = 0.f;

public:
	void ApplyArmRecoil(float DeltaTime);
	void RecoverArmRecoil(float DeltaTime);
	void UpdateArmRecoil(float DeltaTime);
#pragma endregion
};
