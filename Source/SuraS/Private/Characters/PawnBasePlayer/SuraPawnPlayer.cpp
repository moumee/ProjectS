// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PawnBasePlayer/SuraPawnPlayer.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ActorComponents/AttackComponents/ACPlayerAttackTokens.h"
#include "ActorComponents/DamageComponent/ACDamageSystem.h"
#include "ActorComponents/UISystem/ACUIMangerComponent.h"
#include "ActorComponents/WeaponSystem/ACWeapon.h"
#include "ActorComponents/WeaponSystem/WeaponAimUIWidget.h"
#include "Camera/CameraComponent.h"
#include "Characters/PawnBasePlayer/SuraPlayerCameraComponent.h"
#include "Characters/PawnBasePlayer/SuraPlayerMovementComponent.h"
#include "Components/CapsuleComponent.h"

#include "ActorComponents/WeaponSystem/WeaponSystemComponent.h"
#include "Widgets/Player/PlayerHitWidget.h"

ASuraPawnPlayer::ASuraPawnPlayer()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Component"));
	RootComponent = CapsuleComponent;
	CapsuleComponent->SetSimulatePhysics(false);
	CapsuleComponent->SetCapsuleSize(40.f, 90.f);
	CapsuleComponent->SetCollisionProfileName(TEXT("Pawn"));
	CapsuleComponent->SetNotifyRigidBodyCollision(true);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CapsuleComponent);
	Camera->SetRelativeLocation(FVector(0.f, 0.f, 70.f));
	Camera->bUsePawnControlRotation = true;
	Camera->PostProcessSettings.bOverride_DepthOfFieldFocalDistance = true;

	ArmMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Arm Mesh"));
	ArmMesh->SetupAttachment(Camera);

	HandsMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hands Mesh"));
	HandsMesh->SetupAttachment(ArmMesh);
	HandsMesh->SetLeaderPoseComponent(ArmMesh);

	MovementComponent = CreateDefaultSubobject<USuraPlayerMovementComponent>(TEXT("Movement Component"));
	MovementComponent->UpdatedComponent = RootComponent;
	MovementComponent->SetDefaultCapsuleValues(CapsuleComponent->GetScaledCapsuleRadius(), CapsuleComponent->GetScaledCapsuleHalfHeight());

	CameraMovementComponent = CreateDefaultSubobject<USuraPlayerCameraComponent>(TEXT("CameraMovement Component"));
	
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// <WeaponSystem>
	WeaponSystem = CreateDefaultSubobject<UWeaponSystemComponent>(TEXT("WeaponSystem"));
	CapsuleComponent->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
	ArmMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// for damage interactions with enemies
	AttackTokensComponent = CreateDefaultSubobject<UACPlayerAttackTokens>(TEXT("Attack Tokens Component"));
	DamageSystemComponent = CreateDefaultSubobject<UACDamageSystem>(TEXT("Damage System Component"));

	// Hit Effect Class Init - by Yoony
	static ConstructorHelpers::FClassFinder<UPlayerHitWidget> WidgetClass{ TEXT("/Game/UI/Player/WBP_PlayerHit") };

	// UIManager actor components - suhyeon
	UIManager = CreateDefaultSubobject<UACUIMangerComponent>(TEXT("UI Manager Component"));

	if (WidgetClass.Succeeded())
		HitEffectWidgetClass = WidgetClass.Class;
}

void ASuraPawnPlayer::BeginPlay()
{
	Super::BeginPlay();

	GetDamageSystemComponent()->OnDamaged.AddUObject(this, &ASuraPawnPlayer::OnDamaged);
	GetDamageSystemComponent()->OnDeath.AddUObject(this, &ASuraPawnPlayer::OnDeath);

	// Hit Effect Widget Init - by Yoony
	if (IsValid(HitEffectWidgetClass))
	{
		HitEffectWidget = Cast<UPlayerHitWidget>(CreateWidget<UPlayerHitWidget>(GetWorld(), HitEffectWidgetClass));
		
		if (IsValid(HitEffectWidget))
		{
			HitEffectWidget->AddToViewport();
			HitEffectWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	FTimerDelegate PlayerHealthCheckTimerDelegate;
	PlayerHealthCheckTimerDelegate.BindUObject(this, &ASuraPawnPlayer::CheckPlayerHealth);
	GetWorld()->GetTimerManager().SetTimer(PlayerHealthCheckTimer, PlayerHealthCheckTimerDelegate,
		CorrectionSystemCheckTime, true);
}

UCapsuleComponent* ASuraPawnPlayer::GetCapsuleComponent()
{
	return CapsuleComponent;
}

bool ASuraPawnPlayer::HasWeapon() const
{
	if (WeaponSystem)
	{
		return WeaponSystem->GetCurrentWeapon() != nullptr;
	}
	return false;
}

void ASuraPawnPlayer::UpdateLookInputVector2D(const FInputActionValue& InputValue)
{
	PlayerLookInputVector2D = InputValue.Get<FVector2D>();
}

void ASuraPawnPlayer::SetLookInputVector2DZero()
{
	PlayerLookInputVector2D = FVector2D::ZeroVector;
}


void ASuraPawnPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASuraPawnPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASuraPawnPlayer::HandleMoveInput);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &ASuraPawnPlayer::HandleMoveInput);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASuraPawnPlayer::HandleLookInput);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Started, this, &ASuraPawnPlayer::ToggleRunInput);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ASuraPawnPlayer::StartJumpInput);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ASuraPawnPlayer::StopJumpInput);
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Started, this, &ASuraPawnPlayer::StartDashInput);
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Completed, this, &ASuraPawnPlayer::StopDashInput);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &ASuraPawnPlayer::StartCrouchInput);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &ASuraPawnPlayer::StopCrouchInput);

		// <WeaponSystem>
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASuraPawnPlayer::UpdateLookInputVector2D);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::None, this, &ASuraPawnPlayer::SetLookInputVector2DZero);
	}
}

void ASuraPawnPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ASuraPawnPlayer::CheckPlayerHealth()
{
	// UE_LOG(LogTemp, Display, TEXT("Checked Player Health. Current Player Health : %f"), DamageSystemComponent->GetHealth());
	if (DamageSystemComponent->GetHealth() <= ConditionalPlayerHP)
	{
		// Reduce the number of max tokens in the AttackTokensComponent
		// Change the AI variables according to game documents
	}
}

void ASuraPawnPlayer::HandleMoveInput(const FInputActionValue& Value)
{

	if (!MovementComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Player movement component is not valid!"));
		return;
	}
	
	FVector2D InputVector = Value.Get<FVector2D>();

	const FRotator YawRotation = FRotator(0, GetControlRotation().Yaw, 0);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
	MovementComponent->AddInputVector(ForwardDirection * InputVector.Y);
	MovementComponent->AddInputVector(RightDirection * InputVector.X);
	MovementComponent->SetMovementInputVector(InputVector);
}

void ASuraPawnPlayer::HandleLookInput(const FInputActionValue& Value)
{
	FVector2D InputVector = Value.Get<FVector2D>();

	AddControllerYawInput(InputVector.X);
	AddControllerPitchInput(InputVector.Y);
}

void ASuraPawnPlayer::ToggleRunInput()
{
	if (!MovementComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Player movement component is not valid!"));
		return;
	}
	
	MovementComponent->ToggleRunPressed();
}


void ASuraPawnPlayer::StartJumpInput()
{
	if (!MovementComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Player movement component is not valid!"));
		return;
	}
	
	MovementComponent->SetJumpPressed(true);
}

void ASuraPawnPlayer::StopJumpInput()
{
	if (!MovementComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Player movement component is not valid!"));
		return;
	}
	
	MovementComponent->SetJumpPressed(false);
}



void ASuraPawnPlayer::StartDashInput()
{
	if (!MovementComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Player movement component is not valid!"));
		return;
	}

	MovementComponent->SetDashPressed(true);
}

void ASuraPawnPlayer::StopDashInput()
{
	if (!MovementComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Player movement component is not valid!"));
		return;
	}

	MovementComponent->SetDashPressed(false);
}

void ASuraPawnPlayer::StartCrouchInput()
{
	if (!MovementComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Player movement component is not valid!"));
		return;
	}
	
	MovementComponent->SetCrouchPressed(true);
}

void ASuraPawnPlayer::StopCrouchInput()
{
	if (!MovementComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("Player movement component is not valid!"));
		return;
	}
	
	MovementComponent->SetCrouchPressed(false);
}

bool ASuraPawnPlayer::TakeDamage(const FDamageData& DamageData, const AActor* DamageCauser)
{
	return GetDamageSystemComponent()->TakeDamage(DamageData, DamageCauser);
}

UWeaponAimUIWidget* ASuraPawnPlayer::GetWeaponAimUIWidget() const
{
	AWeapon* EquippedWeapon = WeaponSystem->GetCurrentWeapon();
	
	return EquippedWeapon ? EquippedWeapon->GetAimUIWidget() : nullptr; // 현재 장착한 무기의 aimuiwidget을 반환
}

void ASuraPawnPlayer::OnDamaged()
{
	HitEffectWidget->SetVisibility(ESlateVisibility::Visible);
	HitEffectWidget->PlayFadeAnimtion();
}

void ASuraPawnPlayer::OnDeath()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Player Dead"));
}






