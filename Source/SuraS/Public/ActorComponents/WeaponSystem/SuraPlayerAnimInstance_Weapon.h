

#pragma once

#include "CoreMinimal.h"
#include "Characters/Player/SuraPlayerEnums.h"
#include "ActorComponents/WeaponSystem/WeaponStateType.h"
#include "Animation/AnimInstance.h"
#include "SuraPlayerAnimInstance_Weapon.generated.h"

class USuraPlayerBaseState;
class ASuraCharacterPlayerWeapon;
class UACWeapon;
/**
 * 
 */
UCLASS()
class SURAS_API USuraPlayerAnimInstance_Weapon : public UAnimInstance
{
	GENERATED_BODY()
public:
	//SuraPlayerAnimInstance_Weapon();
	//~SuraPlayerAnimInstance_Weapon();

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly, Category = "Player")
	ASuraCharacterPlayerWeapon* Character;


	//-------------------------------------------------
	// <��ȯ�� Player logic ����>

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	FVector Velocity;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float RunSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bIsInAir;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool bCrouchTriggered;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float Direction;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float Pitch;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	USuraPlayerBaseState* CurrentState;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	EPlayerState CurrentStateType;


	//-------------------------------------------------
	// <������ Weapon system ����>

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	EWeaponStateType CurrentWeaponStateType;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	bool bIsZoomIn;

	//TODO: ������ ��. �������
	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	FVector ScreenCenterWorldLocation;

	//TODO: ������ ��. �������
	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	FVector TargetRightHandWorldLocation;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	FTransform AimSocketRelativeTransform;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	FTransform AimPointRelativeTransform;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	FVector AimPointLocation;

	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	FRotator AimPointRotation;

	//TODO: Logic �ٲ�� ��
	UPROPERTY(BlueprintReadOnly, Category = "Weapon")
	UACWeapon* CurrentWeapon;

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
};
