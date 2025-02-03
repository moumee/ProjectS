

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"

#include "ActorComponents/WeaponSystem/WeaponName.h"
#include "ActorComponents/WeaponSystem/WeaponType.h"
#include "ActorComponents/WeaponSystem/WeaponFireMode.h"

#include "ACWeapon.generated.h"

//TODO: Character Class�� ASuraCharacterBase�� ���� ����غ�����
// �ٵ� �켱�� ��ӹ��� Ŭ������ ����
//class ASuraCharacterPlayer
class ASuraCharacterPlayerWeapon;
class USuraWeaponBaseState;
class USuraWeaponIdleState;
class USuraWeaponFiringState;
class USuraWeaponUnequippedState;

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SURAS_API UACWeapon : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:
	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class ASuraProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector MuzzleOffset;

	/** MappingContext */
	//TODO: FireMapping�� Character���� ó���ϴ� ���� ������ ����غ�����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* FireMappingContext;

	/** Fire Input Action */
	//TODO: FireMapping�� Character���� ó���ϴ� ���� ������ ����غ�����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* FireAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ZoomAction;

	struct FInputBindingHandle* LeftMouseButtonActionBinding;

	struct FInputBindingHandle* RightMouseButtonActionBinding;

	/** Sets default values for this component's properties */
	UACWeapon();

	void InitializeWeapon(ASuraCharacterPlayerWeapon* NewCharacter);

	/** Attaches the actor to a FirstPersonCharacter */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool AttachWeaponToPlayer(ASuraCharacterPlayerWeapon* TargetCharacter);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void DetachWeaponFromPlayer();

	/** Make the weapon Fire a Projectile */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Fire();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void StopFire();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void ZoomToggle();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void ZoomIn();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void ZoomOut();


protected:
	// Called when the game starts
	UFUNCTION()
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	/** Ends gameplay for this component. */
	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

#pragma region WeaponState

protected:
	UPROPERTY(VisibleAnywhere)
	USuraWeaponBaseState* CurrentState;

	UPROPERTY(VisibleAnywhere)
	USuraWeaponIdleState* IdleState;

	UPROPERTY(VisibleAnywhere)
	USuraWeaponFiringState* FiringState;

	UPROPERTY(VisibleAnywhere)
	USuraWeaponUnequippedState* UnequippedState;

public:
	void ChangeState(USuraWeaponBaseState* NewState);

#pragma endregion

protected:
	/** The Character holding this weapon*/
	ASuraCharacterPlayerWeapon* Character;
	

#pragma region Aim

protected:

	FTransform AimSocketRelativeTransform;

	bool bIsZoomIn = false;

	FVector RightHandToAimSocketOffset;

protected:
	float LineTraceMaxDistance = 10000.f;

	FVector TargetLocationOfProjectile;

public:
	bool PerformLineTrace(FVector StartLocation, FVector LineDirection, float MaxDistance, FVector& HitLocation);

	void PerformMultiLineTrace(FVector StartLocation, FVector EndLocation);

	void SetAimSocketTransform();

	void SetAimSocketRelativeTransform();

	FTransform GetAimSocketRelativeTransform();


#pragma endregion


#pragma region Equip/Unequip

public:
	void EquipWeapon(ASuraCharacterPlayerWeapon* TargetCharacter);

	void UnequipWeapon(ASuraCharacterPlayerWeapon* TargetCharacter);


#pragma endregion

#pragma region UI

protected:
	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = "Weapon|CrosshairWidget")
	TSubclassOf<UUserWidget> CrosshairWidgetClass;

	// ������ ���� �ν��Ͻ�
	UPROPERTY()
	UUserWidget* CrosshairWidget;

public:

	void ActivateCrosshairWidget(bool bflag);

#pragma endregion

#pragma region WeaponType

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponName WeaponName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponType WeaponType;

public:
	EWeaponName GetWeaponName() const { return WeaponName; }

#pragma endregion

#pragma region FireMode

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponFireMode WeaponFireMode;

	//<Burst Mode>
	FTimerHandle BurstShotTimer;

	float BurstShotFireRate = 0.05f; //TODO: �ϴ� �ӽ� ����

	const int32 BurstShotCount = 5;

	int32 BurstShotFired = 0;

protected: //���� ����
	void HandleSingleFire();

	void HandleBurstFire();

	void HandleFullAutoFire();

	// <Busrt Shot>
	void StartBurstFire();

	void StopBurstFire();

#pragma endregion


};