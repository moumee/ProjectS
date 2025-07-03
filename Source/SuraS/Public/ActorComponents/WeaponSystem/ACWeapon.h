

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"

#include "ActorComponents/WeaponSystem/WeaponName.h"
#include "ActorComponents/WeaponSystem/WeaponAction.h"
#include "ActorComponents/WeaponSystem/WeaponType.h"
#include "ActorComponents/WeaponSystem/WeaponFireMode.h"
#include "ActorComponents/WeaponSystem/WeaponCamSettingValue.h"
#include "ActorComponents/WeaponSystem/WeaponInterface.h"
#include "ActorComponents/WeaponSystem/WeaponRecoilStruct.h"
#include "ActorComponents/WeaponSystem/ArmRecoilStruct.h"
#include "ActorComponents/WeaponSystem/ProjectileSpreadValue.h"

#include "Engine/DataTable.h"
#include "WeaponData.h"

#include "ACWeapon.generated.h"

//class ASuraCharacterPlayerWeapon;
class ASuraPawnPlayer;

class USuraWeaponBaseState;
class USuraWeaponIdleState;
class USuraWeaponFiringState;
class USuraWeaponUnequippedState;
class USuraWeaponReloadingState;
class USuraWeaponSwitchingState;
class USuraWeaponTargetingState;
class USuraWeaponChargingState;
class UWeaponCameraShakeBase;

class UNiagaraSystem;
class UWidgetComponent;
class UAmmoCounterWidget;
class UWeaponAimUIWidget;

class UInputAction;
struct FInputBindingHandle;

//UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
UCLASS()
class SURAS_API AWeapon : public AActor, public IWeaponInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon)
	FDataTableRowHandle WeaponDataTableHandle;
	FWeaponData* WeaponData;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "WeaponMesh")
	USkeletalMeshComponent* WeaponMesh;
	UFUNCTION()
	USkeletalMeshComponent* GetWeaponMesh() { return WeaponMesh; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	EWeaponAction LeftMouseAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Action")
	EWeaponAction RightMouseAction;

	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class ASuraProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	TSubclassOf<class ASuraProjectile> LeftProjectileClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	TSubclassOf<class ASuraProjectile> RightProjectileClass;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector MuzzleOffset;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* FireMappingContext;

	/** Fire Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LeftSingleShotAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* RightSingleShotAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LeftBurstShotAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* RightBurstShotAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LeftFullAutoShotAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* RightFullAutoShotAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LeftHoldAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* RightHoldAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LeftChargeAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* RightChargeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* RightZoomAction;



	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ReloadAction;
	
	TArray<FInputBindingHandle*> InputActionBindingHandles;

	AWeapon();

	void InitializeWeapon(ASuraPawnPlayer* NewCharacter);
	void InitializeCamera(ASuraPawnPlayer* NewCharacter);
	void InitializeUI();

	void LoadWeaponData();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	bool AttachWeaponToPlayer(ASuraPawnPlayer* TargetCharacter);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void DetachWeaponFromPlayer();

	/** Make the weapon Fire a Projectile */
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void FireSingleProjectile(const TSubclassOf<ASuraProjectile>& InProjectileClass, int32 NumPenetrable = 0, int32 AmmoCost = 1, float AdditionalDamage = 0.f, float AdditionalRecoilAmountPitch = 0.f, float AdditionalRecoilAmountYaw = 0.f, float AdditionalProjectileRadius = 0.f, bool bIsHoming = false, AActor* HomingTarget = nullptr);
	void FireMultiProjectile(const TSubclassOf<ASuraProjectile>& InProjectileClass, int32 NumPenetrable = 0, int32 AmmoCost = 1, float AdditionalDamage = 0.f, float AdditionalRecoilAmountPitch = 0.f, float AdditionalRecoilAmountYaw = 0.f, float AdditionalProjectileRadius = 0.f, int32 AdditionalPellet = 0, bool bIsHoming = false, AActor* HomingTarget = nullptr);
	void SpawnProjectile();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void ZoomToggle();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void ZoomIn();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void ZoomOut();

protected:
	UFUNCTION()
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	//virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	/** Ends gameplay for this component. */
	UFUNCTION()
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

#pragma region WeaponState
protected: //TODO: public
	UPROPERTY(VisibleAnywhere)
	USuraWeaponBaseState* CurrentState;

	UPROPERTY(VisibleAnywhere)
	USuraWeaponIdleState* IdleState;

	UPROPERTY(VisibleAnywhere)
	USuraWeaponFiringState* FiringState;

	UPROPERTY(VisibleAnywhere)
	USuraWeaponUnequippedState* UnequippedState;

	UPROPERTY(VisibleAnywhere)
	USuraWeaponReloadingState* ReloadingState;

	UPROPERTY(VisibleAnywhere)
	USuraWeaponSwitchingState* SwitchingState;

	UPROPERTY(VisibleAnywhere)
	USuraWeaponTargetingState* TargetingState;

	UPROPERTY(VisibleAnywhere)
	USuraWeaponChargingState* ChargingState;

public:
	UFUNCTION()
	USuraWeaponBaseState* GetCurrentState() const { return CurrentState; }

	void ChangeState(USuraWeaponBaseState* NewState);

#pragma region suhyeon
	void SetWeaponData(FWeaponData* InWeaponData);

	void SetWeaponName(EWeaponName InWeaponName)
	{
		WeaponName = InWeaponName;
	}

	// 캐릭터 설정 함수
	void SetCharacter(class ASuraCharacterPlayerWeapon* InCharacter);
#pragma endregion
	
#pragma endregion

protected:
	/** The Character holding this weapon*/
	ASuraPawnPlayer* Character;
	
	APlayerController* CharacterController;
		
#pragma region Animation
protected:
	void StartFireAnimation(UAnimMontage* CharacterFireAnimation, UAnimMontage* WeaponFireAnimation);
	void StartAnimation(UAnimMontage* CharacterAnimation, UAnimMontage* WeaponAnimation, float CharacterAnimPlayRate, float WeaponAnimPlayRate);
	void CancelAnimation(UAnimMontage* CharacterAnimation, UAnimMontage* WeaponAnimation);
#pragma endregion

#pragma region Animation/Character
protected:
	UPROPERTY()
	UAnimInstance* CharacterAnimInstance;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation|Character")
	UAnimMontage* AM_Fire_Character;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation|Character")
	UAnimMontage* AM_Reload_Character;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation|Character")
	UAnimMontage* AM_Equip_Character;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation|Character")
	UAnimMontage* AM_Unequip_Character;
#pragma endregion

#pragma region Animation/Weapon
protected:
	UPROPERTY()
	UAnimInstance* WeaponAnimInstance;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation|Weapon")
	UAnimMontage* AM_Fire_Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation|Weapon")
	UAnimMontage* AM_Reload_Weapon;
#pragma endregion

#pragma region Sound
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* FireSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* ChargeSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	UAudioComponent* ChargeAudioComponent;

	void PlayChargeSound();
	void StopChargeSound();
#pragma endregion

#pragma region Niagara
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	UNiagaraSystem* MuzzleFireEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	UNiagaraSystem* ChargeEffect;

	FVector ChargeEffectLocation;
	FRotator ChargeEffectRotation;
	FVector ChargeEffenctScale;

	UPROPERTY()
	UNiagaraComponent* ChargeEffectComponent;
public:
	void SpawnMuzzleFireEffect(FVector SpawnLocation, FRotator SpawnRotation);
	void SpawnChargeEffect(FVector SpawnLocation, FRotator SpawnRotation, FVector EffectScale);
	void DestroyChargeEffect();
#pragma endregion

#pragma region Aim
protected:
	FTransform AimSocketRelativeTransform;

	bool bIsZoomIn = false;

	FVector RightHandToAimSocketOffset;


	FTransform RightHandOffset;
protected:
	float LineTraceMaxDistance = 10000.f;

	UPROPERTY(EditAnywhere)
	float SphereTraceRadius = 20.f;

	FVector ScreenCenterWorldPosition;
	FVector ScreenCenterWorldDirection;

	FVector TargetLocationOfProjectile;

public:
	bool PerformLineTrace(FVector StartLocation, FVector LineDirection, float MaxDistance, FVector& HitLocation);
	bool PerformSphereTrace(FVector StartLocation, FVector TraceDirection, float MaxDistance, float SphereRadius, FVector& HitLocation);

	FVector CalculateScreenCenterWorldPositionAndDirection(FVector& OutWorldPosition, FVector& OutWorldDirection) const;

	void SetAimSocketTransform();
	void SetAimSocketRelativeTransform();
	FTransform GetAimSocketRelativeTransform();
#pragma endregion

#pragma region Equip/Unequip
protected:
	UPROPERTY(EditAnywhere)
	float WeaponSwitchingRate = 1.f;
	FTimerHandle SwitchingTimer;
public:
	void SwitchWeapon(ASuraPawnPlayer* TargetCharacter, bool bEquip);
	void EndWeaponSwitch(ASuraPawnPlayer* TargetCharacter, bool bEquip);
	void EquipWeapon(ASuraPawnPlayer* TargetCharacter);
	void UnequipWeapon(ASuraPawnPlayer* TargetCharacter);
	void SetInputActionBinding();
	void ResetInputActionBinding();
#pragma endregion

#pragma region Reload
protected:
	UPROPERTY(EditAnywhere)
	bool bCanAutoReload = true;

	UPROPERTY(EditAnywhere)
	float ReloadingTime = 2.5f;

	UPROPERTY(EditAnywhere)
	int32 MaxTotalAmmo = 200.f;

	UPROPERTY(EditAnywhere)
	int32 TotalAmmo = 100.f;

	UPROPERTY(EditAnywhere)
	int32 MaxAmmoPerMag = 20.f;

	UPROPERTY(EditAnywhere)
	int32 LeftAmmoInCurrentMag;

	UPROPERTY(EditAnywhere)
	int32 AmmoConsumedPerShot_Left = 1;
	UPROPERTY(EditAnywhere)
	int32 AmmoConsumedPerShot_Right = 1;

	FTimerHandle ReloadingTimer;
protected:
	void HandleReload();
	void CancelReload();
public:
	void StartReload();
protected:
	void StopReload();

	void ConsumeAmmo(int32 AmmoCost = 1);
	void ReloadAmmo();
	bool HasAmmoInCurrentMag();
	bool HasAmmoInCurrentMag(int32 AmmoCost);
public:
	bool AddAmmo(int32 NumAmmo);
	void AutoReload();
	virtual void ReloadingEnd() override; //Legacy
#pragma endregion

#pragma region UI
protected:
	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = "Weapon|CrosshairWidget")
	TSubclassOf<UUserWidget> CrosshairWidgetClass;

	UPROPERTY()
	UUserWidget* CrosshairWidget;

	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = "AimUIWidget")
	TSubclassOf<UWeaponAimUIWidget> AimUIWidgetClass;

	UPROPERTY()
	UWeaponAimUIWidget* AimUIWidget;

	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = "Weapon|AmmoCounterWidget")
	//TSubclassOf<UUserWidget> AmmoCounterWidgetClass;
	TSubclassOf<UAmmoCounterWidget> AmmoCounterWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|AmmoCounterWidget")
	UWidgetComponent* AmmoCounterWidgetComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|AmmoCounterWidget")
	UAmmoCounterWidget* AmmoCounterWidget;

	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = "Weapon|TargetMarkerWidget")
	TSubclassOf<UUserWidget> TargetMarkerWidgetClass;

	//-----------------------------------------------------------
	//TODO: for 3D UI Test
	UPROPERTY()
	UTextureRenderTarget2D* RenderTarget;

	//TODO: 3D UI Test
	UPROPERTY()
	UMaterialInstanceDynamic* WidgetMaterialInstance;

	void Create3DUI();
public:
	void ActivateCrosshairWidget(bool bflag);
	void ActivateAmmoCounterWidget(bool bflag);
	UAmmoCounterWidget* GetAmmoCounterWidget() const { return AmmoCounterWidget; } // suhyeon
	
protected:
	void SetUpAimUIDelegateBinding(ASuraProjectile* Projectile);
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
protected:
	void HandleSingleFire(bool bIsLeftInput = true, bool bSingleProjectile = true, int32 NumPenetrable = 0);
	void HandleBurstFire(bool bIsLeftInput = true, bool bSingleProjectile = true, int32 NumPenetrable = 0);
	void HandleFullAutoFire();
#pragma endregion

#pragma region FireMode/SingleShot
protected:
	FTimerHandle SingleShotTimer;

	UPROPERTY(EditAnywhere)
	float SingleShotDelay = 1.f;
public:
	void StartSingleShot(bool bIsLeftInput = true, bool bSingleProjectile = true, int32 NumPenetrable = 0, float AdditionalDamage = 0.f, float AdditionalRecoilAmountPitch = 0.f, float AdditionalRecoilAmountYaw = 0.f, float AdditionalProjectileRadius = 0.f, int32 AdditionalPellet = 0);
	void StopSingleShot();
#pragma endregion

#pragma region FireMode/BurstShot
protected:
	FTimerHandle BurstShotTimer;

	UPROPERTY(EditAnywhere, Category = "BurstShot")
	float BurstShotDelay = 1.f;

	UPROPERTY(EditAnywhere)
	float BurstShotFireRate = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BurstShotCount = 3;

	int32 BurstShotFired = 0;

protected:
	void StartBurstFire(bool bIsLeftInput = true, bool bSingleProjectile = true, int32 NumPenetrable = 0, float AdditionalDamage = 0.f, float AdditionalRecoilAmountPitch = 0.f, float AdditionalRecoilAmountYaw = 0.f, float AdditionalProjectileRadius = 0.f);
	void StopBurstFire();
	void EndBurstShot();
#pragma endregion

#pragma region FireMode/FullAuto
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|FireMode")
	float FullAutoShotFireRate = 0.1f;

	FTimerHandle FullAutoShotTimer;

protected:
	void StartFullAutoShot(bool bIsLeftInput = true, bool bSingleProjectile = true, int32 NumPenetrable = 0);
	void UpdateFullAutoShot(bool bIsLeftInput = true, bool bSingleProjectile = true, int32 NumPenetrable = 0);
	void StopFullAutoShot();
#pragma endregion

#pragma region FireMode/Targeting
protected:
	UPROPERTY()
	TArray<UUserWidget*> TargetMarkerWidgets;
	UPROPERTY()
	TSet<AActor*> Targets;

	UPROPERTY()
	TMap<AActor*, UUserWidget*> MapTargetActorToWidget;

	FTimerHandle TargetDetectionTimer;

	int32 MaxTargetNum = 10;

	float MaxTargetDetectionRadius = 3000.f;
	float MaxTargetDetectionAngle = 80.f;

	float CurrentTargetDetectionRadius = 0.f;
	float CurrentTargetDetectionAngle = 0.f;

	float MaxTargetDetectionTime = 8.f;
	float TimeToReachMaxTargetDetectionRange = 2.5f;
	float ElapsedTimeAfterTargetingStarted = 0.f;
protected:
	void StartTargetDetection();
	void UpdateTargetDetection(float DeltaTime);
	void StopTargetDetection(const TSubclassOf<ASuraProjectile>& InProjectileClass);

	bool SearchOverlappedActor(FVector CenterLocation, float SearchRadius, TArray<AActor*>& OverlappedActors);
	TTuple<FVector2D, bool> GetScreenPositionOfWorldLocation(const FVector& SearchLocation) const;
	bool IsInViewport(FVector2D ActorScreenPosition, float ScreenRatio_Width = 0.0f, float ScreenRatio_Height = 0.0f) const;
	float GetUnsignedAngleBetweenVectors(const FVector& VectorA, const FVector& VectorB, const FVector& Axis);
	bool CheckIfTargetIsBlockedByObstacle(AActor* target);

	UUserWidget* CreateTargetMarkerWidget(AActor* TargetActor);
public:
	void UpdateTargetMarkers();
	void ResetTargetMarkers();
protected:
	TArray<AActor*> ConfirmedTargets;
	int32 CurrentTargetIndex = 0;
	float MissileLaunchDelay = 0.2;
	FTimerHandle MissileLaunchTimer;
protected:
	void StartMissileLaunch(TArray<AActor*> TargetActors, const TSubclassOf<ASuraProjectile>& InProjectileClass);
	void UpdateMissileLaunch(const TSubclassOf<ASuraProjectile>& InProjectileClass);
	void StopMissileLaunch();
#pragma endregion

#pragma region FireMode/Charging
protected:
	bool bAutoFireAtMaxChargeTime = true;
	float ChargeTimeThreshold = 0.5f;
	float MaxChargeTime = 3.f;
	float ChargingAdditionalDamageBase = 100.f;

	float ChargingAdditionalRecoilAmountPitchBase = 4.f;
	float ChargingAdditionalRecoilAmountYawBase = 1.f;
	float ChargingAdditionalProjectileRadiusBase = 20.f;

	int32 ChargingAdditionalPelletMaxNum = 0;
	//int32 MaxPenetrableObjectsNum = 4;

	float ElapsedChargeTime = 0.f;
	FTimerHandle ChargingTimer;
protected:
	void StartCharge();
	void UpdateCharge();
	void StopCharge();
#pragma endregion

#pragma region
protected:
	int32 MaxPenetrableObjectsNum_Left = 4;
	int32 MaxPenetrableObjectsNum_Right = 4;
#pragma endregion

#pragma region Recoil/Aim
protected:
	bool bIsRecoiling = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWeaponRecoilStruct DefaultRecoil;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWeaponRecoilStruct ZoomRecoil;

	FTimerHandle RecoilRecoverTimer;

	float TotalTargetRecoilValuePitch = 0.f;
	float TotalTargetRecoilValueYaw = 0.f;

	float CulmulatedRecoilValuePitch = 0.f;
	float CulmulatedRecoilValueYaw = 0.f;

	float RecoveredRecoilValuePitch = 0.f;
	float RecoveredRecoilValueYaw = 0.f;
public:
	void AddRecoilValue(FWeaponRecoilStruct* RecoilStruct = nullptr, float AdditionalRecoilAmountPitch = 0.f, float AdditionalRecoilAmountYaw = 0.f);
	void ApplyRecoil(float DeltaTime, FWeaponRecoilStruct* RecoilStruct = nullptr);
	void RecoverRecoil(float DeltaTime, FWeaponRecoilStruct* RecoilStruct = nullptr);
	void UpdateRecoil(float DeltaTime);
#pragma endregion

#pragma region Recoil/ArmAnimation
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ArmRecoil")
	FArmRecoilStruct ArmRecoil_Hand;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ArmRecoil")
	FArmRecoilStruct ArmRecoil_UpperArm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ArmRecoil")
	FArmRecoilStruct ArmRecoil_LowerArm;
public:
	void AddArmRecoil();

	FArmRecoilStruct* GetArmRecoilInfo_Hand();
	FArmRecoilStruct* GetArmRecoilInfo_UpperArm();
	FArmRecoilStruct* GetArmRecoilInfo_LowerArm();
#pragma endregion


#pragma region Overheat
protected:
	UPROPERTY(EditAnywhere)
	bool bIsOverheatMode = false;
	UPROPERTY(EditAnywhere)
	float OverheatBaseIncrement = 0.5f; //TODO: 총기 능력 사용 시간만큼 더해지도록 해야함. 그리고 우클릭 좌클릭 다르게 들어가도록
	UPROPERTY(EditAnywhere)
	float MaxOverheatValue = 5.f;
	UPROPERTY(EditAnywhere)
	float OverheatSpeed = 10.f;
	UPROPERTY(EditAnywhere)
	float OverheatRecoverSpeed = 10.f; //TODO: Reload Time을 사용할 수도 있어서 조금 지켜봐야함

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float OverheatRecoveryStartTime = 0.2f;

	bool bIsOverheating = false;

	float TotalTargetOverheatValue = 0.f;
	float CurrentOverheatVaule = 0.f;
	float OverheatRecoverTimer = 0.f;
protected:
	void AddOverheatValue();
	void ApplyOverheat(float DeltaTime);
	void RecoverOverheat(float DeltaTime);
	void UpdateOverheat(float DeltaTime);


#pragma endregion

#pragma region Projectile/SingleProjectileSpread
protected:
	bool bIsSpreading = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileSpread")
	FProjectileSpreadValue DefaultSpread;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProjectileSpread")
	FProjectileSpreadValue ZoomSpread;

	float TotalTargetSpreadValue = 0.f;
	float CurrentSpreadVaule = 0.f;
	float SpreadRecoverTimer = 0.f;
protected:
	void AddSpreadValue(FProjectileSpreadValue* SpreadValue = nullptr);
	void ApplySpread(float DeltaTime, FProjectileSpreadValue* SpreadValue = nullptr);
	void RecoverSpread(float DeltaTime, FProjectileSpreadValue* SpreadValue = nullptr);
	void UpdateSpread(float DeltaTime);
	FVector GetRandomSpreadVector(FVector BaseDir);
#pragma endregion

#pragma region Projectile/MultiProjectileSpread
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MultiProjectile")
	bool bEnableMultiProjectile_Left;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MultiProjectile")
	bool bEnableMultiProjectile_Right;

	UPROPERTY(EditAnywhere)
	int32 PelletsNum = 9;

	UPROPERTY(EditAnywhere)
	float MaxAngleOfMultiProjectileSpread = 15.f;
#pragma endregion

#pragma region Camera
protected:
	bool bIsUsingPlayerCamFov = false;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWeaponCamSettingValue CamSetting_Default = FWeaponCamSettingValue(
		90.f,
		{ 0.f, 0.f, 0.f },
		{ 0.f, 0.f, 70.f },
		1.f, 1.f, 15.f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWeaponCamSettingValue CamSetting_ZoomIn = FWeaponCamSettingValue(
		90.f,
		{ 0.f, 0.f, 0.f },
		{ 50.f, 0.f, 70.f },
		1.f, 1.f, 15.f);
	FTimerHandle CamSettingTimer;
public:
	void StartCameraSettingChange(FWeaponCamSettingValue* CamSetting);
	void UpdateCameraSetting(float DeltaTime, FWeaponCamSettingValue* CamSetting);
	void StopCameraSettingChange();
	void ForceStopCamModification();
	bool IsModifyingPlayerCamFov() const { return bIsUsingPlayerCamFov; }
#pragma endregion

#pragma region CameraShake
protected:
	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = "CameraShake")
	TSubclassOf<UWeaponCameraShakeBase> DefaultCameraShakeClass;

	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = "CameraShake")
	TSubclassOf<UWeaponCameraShakeBase> ZoomCameraShakeClass;

	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = "CameraShake")
	TSubclassOf<UWeaponCameraShakeBase> ChargingCameraShakeClass;
public:
	void ApplyCameraShake(TSubclassOf<UWeaponCameraShakeBase> CamShakeClass = nullptr, float Scale = 1.f);
#pragma endregion
};