// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/Enemies/SuraCharacterEnemyBase.h"

#include "BrainComponent.h"
#include "SuraSProjectile.h"
#include "ActorComponents/DamageComponent/ACEnemyDamageSystem.h"
#include "ActorComponents/UISystem/ACKillLogManager.h"
#include "ActorComponents/WeaponSystem/ProjectileType.h"
#include "ActorComponents/WeaponSystem/SuraCharacterPlayerWeapon.h"
#include "Characters/Enemies/AI/EnemyBaseAIController.h"
#include "Characters/PawnBasePlayer/SuraPawnPlayer.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Widgets/Enemies/EnemyHealthBarWidget.h"
#include "Structures/Enemies/EnemyAttributesData.h"
#include "Weapons/SuraEnemyWeapon.h"

ASuraCharacterEnemyBase::ASuraCharacterEnemyBase()
{
	// Damage system comp
	DamageSystemComp = CreateDefaultSubobject<UACEnemyDamageSystem>(TEXT("DamageSystemComponent"));
	AddOwnedComponent(DamageSystemComp);

	HealthBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));

	if (HealthBarWidget)
	{
		HealthBarWidget->SetupAttachment(RootComponent);
		HealthBarWidget->SetWidgetSpace(EWidgetSpace::World);
		HealthBarWidget->SetRelativeLocation(FVector(0.f, 0.f, 1.f));

		static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass{ TEXT("/Game/UI/Enemies/WBP_EnemyHealthBar") };

		if (WidgetClass.Succeeded())
		{
			HealthBarWidget->SetWidgetClass((WidgetClass.Class));
		}
	}
	
	bUseControllerRotationYaw = true; // for controller controled rotation
	GetCharacterMovement()->bUseRVOAvoidance = true;

	EnemyType = "Base";

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore);
}

void ASuraCharacterEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	InitializeEnemy();
	
	BindKillLogOnDeath();
}

void ASuraCharacterEnemyBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	if (!isInitialized) return;
	
	if (UEnemyHealthBarWidget* const Widget = Cast<UEnemyHealthBarWidget>(HealthBarWidget->GetUserWidgetObject()))
	{
		float Distance = FVector::Dist(GetPlayerController()->PlayerCameraManager->GetCameraLocation(), GetActorLocation());

		//int32 ViewportX, ViewportY;
		//GetPlayerController()->GetViewportSize(ViewportX, ViewportY);
		
		Distance = FMath::Clamp(Distance, 100, 1000);

		FVector2D NewSize = GetHealthBarWidgetSize() * 500 / Distance; // * (ViewportY / 2) / FMath::Tan(FMath::DegreesToRadians(GetPlayerController()->PlayerCameraManager->GetFOVAngle() / 2))

		Widget->ResizeHealthBar(NewSize);
	}

	/*FHitResult Hit;
	
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(Hit, EnemyLocation, CameraLocation, ECollisionChannel::ECC_Visibility, CollisionQueryParams);

	DrawDebugLine(GetWorld(), EnemyLocation, CameraLocation, Hit.bBlockingHit ? FColor::Blue : FColor::Red, false, 5.0f, 0, 10.0f);

	if (Hit.bBlockingHit)
		HealthBarWidget->SetHiddenInGame(true);
	else
		HealthBarWidget->SetHiddenInGame(false);*/
}

void ASuraCharacterEnemyBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

void ASuraCharacterEnemyBase::OnDamagedTriggered()
{
	if (GetWorldTimerManager().IsTimerActive(HideHealthBarHandle))
		GetWorldTimerManager().ClearTimer(HideHealthBarHandle);

	GetWorldTimerManager().SetTimer(
		HideHealthBarHandle,
		FTimerDelegate::CreateLambda([&]() { HealthBarWidget->SetHiddenInGame(true); }),
		1.f,
		false
	);

	UpdateHealthBarValue();
	HealthBarWidget->SetHiddenInGame(false);

	if (!HealthBarWidget->bHiddenInGame)
		Cast<UEnemyHealthBarWidget>(HealthBarWidget->GetUserWidgetObject())->PlayFadeAnimation();

	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%s"), *(HitAnimation->GetFName()).ToString()));

	if (!HitAnimations.IsEmpty())
	{
		OnHitMontageEnded.BindUObject(this, &ASuraCharacterEnemyBase::OnHitEnded);
		
		UAnimInstance* const EnemyAnimInstance = GetMesh()->GetAnimInstance();
		EnemyAnimInstance->Montage_Play(GetRandomAnimationMontage(HitAnimations));

		GetMesh()->GetAnimInstance()->Montage_SetBlendingOutDelegate(OnHitMontageEnded); // montage interrupted
		GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(OnHitMontageEnded); // montage ended
	}
}

void ASuraCharacterEnemyBase::OnHitEnded(UAnimMontage* AnimMontage, bool bInterrupted)
{
	ASuraPawnPlayer* Player = Cast<ASuraPawnPlayer>(GetPlayerController()->GetPawn());

	if (Player && AIController->GetCurrentState() != EEnemyStates::Pursue && AIController->GetCurrentState() != EEnemyStates::Attacking)
		GetAIController()->SetStateToChaseOrPursue(Player);
}

void ASuraCharacterEnemyBase::OnDeathTriggered()
{
	UpdateHealthBarValue();

	float DeathAnimDuration = 3.f;
	
	if (!DeathAnimations.IsEmpty())
	{
		UAnimMontage* DeathAnimation = GetRandomAnimationMontage(DeathAnimations);
		
		PlayAnimMontage(DeathAnimation);
		DeathAnimDuration = DeathAnimation->GetPlayLength();
	}

	if (AIController->GetCurrentState() == EEnemyStates::Pursue || AIController->GetCurrentState() == EEnemyStates::Attacking)
	{
		AIController->EndPursueState();
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Puruse Token Returned"));
	}

	AIController->GetBrainComponent()->StopLogic("Death");

	// Disable all collisions on capsule
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetCapsuleComponent()->SetCollisionObjectType(ECC_GameTraceChannel1); // to disable collision with SuraProjectile object

	GetCapsuleComponent()->SetSimulatePhysics(false);

	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%s"), GetCapsuleComponent()->IsSimulatingPhysics() ? TEXT("true") : TEXT("false")));

	// Ragdoll physics
	/*GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionObjectType(ECC_GameTraceChannel1);*/ // to disable collision with SuraProjectile object

	//objectpoolDisableEnemy
	FTimerHandle DeathHandle;
	
	GetWorldTimerManager().SetTimer(
		DeathHandle,
		FTimerDelegate::CreateLambda([&]()
		{
			/*GetMesh()->SetAnimationMode(EAnimationMode::AnimationSingleNode);
			GetMesh()->Stop();*/
			
			if (EnemyWeapon)
				EnemyWeapon->Destroy();

			Destroy();
		}),
		DeathAnimDuration,
		false
	);
}

void ASuraCharacterEnemyBase::UpdateHealthBarValue()
{
	const float Health = GetDamageSystemComp()->GetHealth();
	const float MaxHealth = GetDamageSystemComp()->GetMaxHealth();

	if (UEnemyHealthBarWidget* const Widget = Cast<UEnemyHealthBarWidget>(HealthBarWidget->GetUserWidgetObject()))
		Widget->SetHealthBarPercent(Health / MaxHealth);
}

UAnimMontage* ASuraCharacterEnemyBase::GetRandomAnimationMontage(TArray<UAnimMontage*> AnimMontages)
{
	int selection = FMath::RandRange(0, AnimMontages.Num() - 1);

	return AnimMontages[selection];
}

void ASuraCharacterEnemyBase::LungeToTarget(float LungeForce = 1000.f)
{
	FVector TargetLocation = GetAIController()->GetAttackTarget()->GetActorLocation();
	FVector MyLocation = GetActorLocation();
	FVector Direction = (TargetLocation - MyLocation).GetSafeNormal();

	// float LungeDistance = FVector::Dist(TargetLocation, MyLocation);
	// FVector NewLocation = MyLocation + Direction * LungeDistance;

	// You could either use interpolation:
	LaunchCharacter(Direction * LungeForce, true, true);
}

bool ASuraCharacterEnemyBase::TakeDamage(const FDamageData& DamageData, const AActor* DamageCauser)
{
	return GetDamageSystemComp()->TakeDamage(DamageData, DamageCauser);
}

void ASuraCharacterEnemyBase::Attack(const ASuraPawnPlayer* Player)
{
	if (!AttackAnimations.IsEmpty())
	{
		UAnimInstance* const EnemyAnimInstance = GetMesh()->GetAnimInstance();
		EnemyAnimInstance->Montage_Play(GetRandomAnimationMontage(AttackAnimations));
	}
}

void ASuraCharacterEnemyBase::SetMovementSpeed(EEnemySpeed Speed)
{
	switch (Speed)
	{
	case EEnemySpeed::Idle:
		GetCharacterMovement()->MaxWalkSpeed = 0.f;
		break;
	case EEnemySpeed::Walk:
		GetCharacterMovement()->MaxWalkSpeed = 250.f;
		break;
	case EEnemySpeed::Jog:
		GetCharacterMovement()->MaxWalkSpeed = 400.f;
		break;
	case EEnemySpeed::Sprint:
		GetCharacterMovement()->MaxWalkSpeed = 800.f;
		break;
	default:
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
		break;
	}
}

void ASuraCharacterEnemyBase::Climb(const FVector& Destination)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("climb"));

	// GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	AddMovementInput(FVector(Destination.X, Destination.Y, Destination.Z), true);

	FOnMontageEnded OnClimbMontageEnded;

	OnClimbMontageEnded.BindUObject(this, &ASuraCharacterEnemyBase::OnClimbEnded);

	if (ClimbAnimation)
	{
		UAnimInstance* const EnemyAnimInstance = GetMesh()->GetAnimInstance();
		
		// SetActorRotation(FRotator(90.f, GetActorRotation().Yaw, GetActorRotation().Roll));
		
		EnemyAnimInstance->Montage_Play(ClimbAnimation);
	}

	GetMesh()->GetAnimInstance()->Montage_SetBlendingOutDelegate(OnClimbMontageEnded); // montage interrupted
	GetMesh()->GetAnimInstance()->Montage_SetEndDelegate(OnClimbMontageEnded); // montage ended
}

void ASuraCharacterEnemyBase::OnClimbEnded(UAnimMontage* AnimMontage, bool bInterrupted)
{
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}

void ASuraCharacterEnemyBase::InitializeEnemy()
{
	if (isInitialized)
	{
		GetDamageSystemComp()->SetHealth(GetDamageSystemComp()->GetMaxHealth());
	}
	else
	{
		UpdateHealthBarValue();

		HealthBarWidget->SetHiddenInGame(true);

		if (UEnemyHealthBarWidget* const Widget = Cast<UEnemyHealthBarWidget>(HealthBarWidget->GetUserWidgetObject()))
		{
			HealthBarWidgetSize = Widget->GetHealthBarSize();
		}

		if (DamageSystemComp)
		{
			GetDamageSystemComp()->OnDamaged.AddUObject(this, &ASuraCharacterEnemyBase::OnDamagedTriggered);
			GetDamageSystemComp()->OnDeath.AddUObject(this, &ASuraCharacterEnemyBase::OnDeathTriggered);
		}

		// GetCapsuleComponent()->SetVisibility(true);
		// GetCapsuleComponent()->SetHiddenInGame(false);

		if (const auto EnemyAttributesData = EnemyAttributesDT.DataTable->FindRow<FEnemyAttributesData>(EnemyType, ""))
		{
			GetDamageSystemComp()->SetMaxHealth(EnemyAttributesData->MaxHealth);
			GetDamageSystemComp()->SetHealth(EnemyAttributesData->MaxHealth);

			GetCharacterMovement()->MaxWalkSpeed = EnemyAttributesData->MaxWalkSpeed;

			AttackDamageAmount = EnemyAttributesData->AttackDamageAmount;
			MeleeAttackRange = EnemyAttributesData->MeleeAttackRange;
			MeleeAttackSphereRadius = EnemyAttributesData->MeleeAttackSphereRadius;

			HitAnimations = EnemyAttributesData->HitAnimations;
			DeathAnimations = EnemyAttributesData->DeathAnimations;
			AttackAnimations = EnemyAttributesData->AttackAnimations;
			ClimbAnimation = EnemyAttributesData->ClimbAnimation;
		}

		PlayerController = GetWorld()->GetFirstPlayerController();

		isInitialized = true;
	}
}

void ASuraCharacterEnemyBase::BindKillLogOnDeath() const
{
	if (UACDamageSystem* DamageSystem = FindComponentByClass<UACDamageSystem>())
	{
		DamageSystem->OnDeath.AddLambda([this]()
		{
			// KillLog 호출 로직
			// 3. 플레이어 가져오기
			APlayerController* PC = GetPlayerController();
			if (!PC) return;

			ASuraPawnPlayer* Player = Cast<ASuraPawnPlayer>(PC->GetPawn());
			if (!Player) return;

			// 4. UIManager → KillLogManager 가져와서 호출
			if (UACUIMangerComponent* UIManager = Player->FindComponentByClass<UACUIMangerComponent>())
			{
				if (UACKillLogManager* KLM = UIManager->GetKillLogManager())
				{
					KLM->AddKillLog(Player->GetName(), this->GetName());
				}
			}
		});
	}
}

void ASuraCharacterEnemyBase::SetUpAIController(AEnemyBaseAIController* const NewAIController)
{
	AIController = NewAIController;
}