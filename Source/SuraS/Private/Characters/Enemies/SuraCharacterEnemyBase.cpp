// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/Enemies/SuraCharacterEnemyBase.h"

#include "BrainComponent.h"
#include "ActorComponents/DamageComponent/ACDamageSystem.h"
#include "Characters/Enemies/AI/EnemyBaseAIController.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Widgets/Enemies/EnemyHealthBarWidget.h"
#include "Structures/Enemies/EnemyAttributesData.h"
#include "Weapons/SuraEnemyWeapon.h"

ASuraCharacterEnemyBase::ASuraCharacterEnemyBase()
{
	// Damage system comp
	DamageSystemComp = CreateDefaultSubobject<UACDamageSystem>(TEXT("DamageSystemComponent"));
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

	EnemyType = "Base";
}

void ASuraCharacterEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	InitializeEnemy();
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
		Cast<UEnemyHealthBarWidget>(HealthBarWidget->GetUserWidgetObject())->PlayFadeAnimtion();

	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%s"), *(HitAnimation->GetFName()).ToString()));

	if (HitAnimation)		
		PlayAnimMontage(HitAnimation);
}

void ASuraCharacterEnemyBase::OnDeathTriggered()
{
	UpdateHealthBarValue();

	if (DeathAnimation)
		PlayAnimMontage(DeathAnimation);

	AIController->GetBrainComponent()->StopLogic("Death");

	// Disable all collisions on capsule
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetCapsuleComponent()->SetCollisionObjectType(ECC_GameTraceChannel1); // to disable collision with SuraProjectile object

	GetCapsuleComponent()->SetSimulatePhysics(false);

	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%s"), GetCapsuleComponent()->IsSimulatingPhysics() ? TEXT("true") : TEXT("false")));

	// Ragdoll physics
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionObjectType(ECC_GameTraceChannel1); // to disable collision with SuraProjectile object

	//objectpoolDisableEnemy
	FTimerHandle DeathHandle;

	GetWorldTimerManager().SetTimer(
		DeathHandle,
		FTimerDelegate::CreateLambda([&]()
		{
			SetActorHiddenInGame(true);
			
			if (EnemyWeapon)
				EnemyWeapon->SetActorHiddenInGame(true);
		}),
		3.f,
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

bool ASuraCharacterEnemyBase::TakeDamage(const FDamageData& DamageData, const AActor* DamageCauser)
{
	return GetDamageSystemComp()->TakeDamage(DamageData, DamageCauser);
}

void ASuraCharacterEnemyBase::Attack(const ASuraCharacterPlayer* Player)
{
	if (AttackAnimation)
	{
		UAnimInstance* const EnemyAnimInstance = GetMesh()->GetAnimInstance();
		EnemyAnimInstance->Montage_Play(AttackAnimation);
	}
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

			HitAnimation = EnemyAttributesData->HitAnimation;
			DeathAnimation = EnemyAttributesData->DeathAnimation;
			AttackAnimation = EnemyAttributesData->AttackAnimation;
		}

		PlayerController = GetWorld()->GetFirstPlayerController();

		isInitialized = true;
	}
}

void ASuraCharacterEnemyBase::SetUpAIController(AEnemyBaseAIController* const NewAIController)
{
	AIController = NewAIController;
}