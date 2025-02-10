// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/Enemies/SuraCharacterEnemyBase.h"

#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "ActorComponents/DamageComponent/ACDamageSystem.h"
#include "Widgets/Enemies/EnemyHealthBarWidget.h"
#include "Structures/Enemies/EnemyAttributesData.h"

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
}

void ASuraCharacterEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	UpdateHealthBarValue();

	HealthBarWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (DamageSystemComp)
	{
		GetDamageSystemComp()->OnDamaged.AddDynamic(this, &ASuraCharacterEnemyBase::OnDamagedTriggered);
		GetDamageSystemComp()->OnDeath.AddDynamic(this, &ASuraCharacterEnemyBase::OnDeathTriggered);
	}

	// GetCapsuleComponent()->SetVisibility(true);
	// GetCapsuleComponent()->SetHiddenInGame(false);

	const auto EnemyAttributesData = EnemyAttributesDT.DataTable->FindRow<FEnemyAttributesData>("Base", "");

	if (EnemyAttributesData)
	{
		GetDamageSystemComp()->SetMaxHealth(EnemyAttributesData->MaxHealth);
		GetDamageSystemComp()->SetHealth(EnemyAttributesData->MaxHealth);

		HitAnimation = EnemyAttributesData->HitAnimation;
		DeathAnimation = EnemyAttributesData->DeathAnimation;
	}
}

void ASuraCharacterEnemyBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	FVector CameraLocation = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraLocation();
	FVector HealthBarLocation = HealthBarWidget->GetComponentLocation();

	FRotator HealthBarTargetRotation = UKismetMathLibrary::FindLookAtRotation(HealthBarLocation, CameraLocation);

	HealthBarWidget->SetWorldRotation(FQuat::MakeFromRotator(HealthBarTargetRotation));
}

void ASuraCharacterEnemyBase::OnDamagedTriggered()
{
	UpdateHealthBarValue();

	if (HitAnimation)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%s"), *(HitAnimation->GetFName()).ToString()));

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		if (AnimInstance)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Blue, TEXT("anim instance found"));
			AnimInstance->Montage_Play(HitAnimation, 1.f);
		}

		// PlayAnimMontage(HitAnimation, 1, NAME_None);
	}
}

void ASuraCharacterEnemyBase::OnDeathTriggered()
{
	UpdateHealthBarValue();

	if (DeathAnimation)
		PlayAnimMontage(DeathAnimation);

	// Disable all collision on capsule
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);
	GetCapsuleComponent()->SetCollisionObjectType(ECC_GameTraceChannel1); // to disable collision with SuraProjectile object

	GetCapsuleComponent()->SetSimulatePhysics(false);

	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("%s"), GetCapsuleComponent()->IsSimulatingPhysics() ? TEXT("true") : TEXT("false")));

	// Ragdoll physics
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionObjectType(ECC_GameTraceChannel1); // to disable collision with SuraProjectile object
}

void ASuraCharacterEnemyBase::UpdateHealthBarValue()
{
	const float Health = GetDamageSystemComp()->GetHealth();
	const float MaxHealth = GetDamageSystemComp()->GetMaxHealth();

	if (auto const widget = Cast<UEnemyHealthBarWidget>(HealthBarWidget->GetUserWidgetObject()))
		widget->SetHealthBarPercent(Health / MaxHealth);
}

bool ASuraCharacterEnemyBase::TakeDamage(FDamageData DamageData, AActor* DamageCauser)
{
	return GetDamageSystemComp()->TakeDamage(DamageData, DamageCauser);
}