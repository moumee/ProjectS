// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Pawn.h"
#include "Interfaces/Damageable.h"
#include "SuraPawnPlayer.generated.h"

class UAmmoCounterWidget;
class USuraPlayerCameraComponent;
class USpringArmComponent;
class USuraPlayerMovementComponent;
class UWeaponSystemComponent;
class UCameraComponent;
class UCapsuleComponent;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;

// for interactions with enemies - must keep - by Yoony
class UACDamageSystem;
class UACPlayerAttackTokens;
class UPlayerHitWidget;
class UACUIMangerComponent;

UCLASS()
class SURAS_API ASuraPawnPlayer : public APawn, public IDamageable
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> HitEffectWidgetClass;

	UPROPERTY()
	UPlayerHitWidget* HitEffectWidget;

public:
	ASuraPawnPlayer();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;
	
	virtual void BeginPlay() override;

	UCapsuleComponent* GetCapsuleComponent();

	UCameraComponent* GetCameraComponent() const { return Camera; };

	UWeaponSystemComponent* GetWeaponSystemComponent() const { return WeaponSystem; }  // <WeaponSystem>

	USkeletalMeshComponent* GetArmMesh() { return ArmMesh; }  // <WeaponSystem>

	bool HasWeapon() const;  // <WeaponSystem>s

	void UpdateLookInputVector2D(const FInputActionValue& InputValue);  // <WeaponSystem>
	void SetLookInputVector2DZero();  // <WeaponSystem>
	FVector2D GetPlayerLookInputVector() const { return PlayerLookInputVector2D; } // <WeaponSystem>
	USuraPlayerMovementComponent* GetPlayerMovementComponent() { return MovementComponent; };

	// for damage system comp and interactions with enemies
	UACDamageSystem* GetDamageSystemComponent() const { return DamageSystemComponent; }
	UACPlayerAttackTokens* GetAttackTokensComponent() const { return AttackTokensComponent; }
	virtual bool TakeDamage(const FDamageData& DamageData, const AActor* DamageCauser) override;

	// SuraPawnPlayer.h - suhyeon
	UFUNCTION(BlueprintCallable)
	UPlayerHitWidget* GetPlayerHitWidget() const {return HitEffectWidget;}

protected:

	UPROPERTY(EditAnywhere)
	TObjectPtr<UCapsuleComponent> CapsuleComponent;
	

	UPROPERTY(EditAnywhere, Category = "Blueprint Assign")
	TObjectPtr<USkeletalMeshComponent> ArmMesh;

	UPROPERTY(EditAnywhere, Category = "Blueprint Assign")
	TObjectPtr<USkeletalMeshComponent> HandsMesh;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USuraPlayerMovementComponent> MovementComponent;

	UPROPERTY(EditAnywhere, Category = "WeaponSystem")
	TObjectPtr<UWeaponSystemComponent> WeaponSystem;  // <WeaponSystem>
	
	// This actor component is for handling camera shakes and state-based movement
	// IT IS NOT THE CAMERA!!
	UPROPERTY(EditAnywhere)
	TObjectPtr<USuraPlayerCameraComponent> CameraMovementComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Damage")
	UACDamageSystem* DamageSystemComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attack Tokens")
	UACPlayerAttackTokens* AttackTokensComponent;

	// UI component - suhyeon
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseUI", meta = (AllowPrivateAccess = "true"))
	UACUIMangerComponent* UIManager;

	// AmmoWidget - suhyeon
	UPROPERTY()
	UAmmoCounterWidget* AmmoWidget;

	UPROPERTY(EditAnywhere, Category = "Blueprint Assign")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, Category = "Blueprint Assign")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = "Blueprint Assign")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, Category = "Blueprint Assign")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, Category = "Blueprint Assign")
	TObjectPtr<UInputAction> ShiftAction;

	UPROPERTY(EditAnywhere, Category = "Blueprint Assign")
	TObjectPtr<UInputAction> CrouchAction;

	FTimerHandle PlayerHealthCheckTimer;
	float ConditionalPlayerHP = 50.f;
	float CorrectionSystemCheckTime = 10.f;
	void CheckPlayerHealth();

	FVector2D PlayerLookInputVector2D; // <WeaponSystem>

	void HandleMoveInput(const FInputActionValue& Value);
	void HandleLookInput(const FInputActionValue& Value);
	void StartJumpInput();
	void StopJumpInput();
	void StartShiftInput();
	void StopShiftInput();
	void StartCrouchInput();
	void StopCrouchInput();
	

	// Damage Comp Event Delegate Functions
	void OnDamaged();
	void OnDeath();
};


