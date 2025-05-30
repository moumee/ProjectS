

#pragma once

#include "CoreMinimal.h"
#include "Characters/PawnBasePlayer/SuraPlayerMovementComponent.h"
#include "ActorComponents/WeaponSystem/WeaponStateType.h"
#include "Animation/AnimInstance.h"
#include "SuraPlayerAnimInstance_Weapon.generated.h"

class USuraPlayerBaseState;
class ASuraPawnPlayer;
class AWeapon;
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

};
