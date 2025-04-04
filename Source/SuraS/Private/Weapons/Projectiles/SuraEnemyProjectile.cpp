// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Projectiles/SuraEnemyProjectile.h"

#include "Interfaces/Damageable.h"
#include "Structures/DamageData.h"
#include "Characters/Player/SuraCharacterPlayer.h"
#include "Structures/Enemies/EnemyProjectileAttributesData.h"

#define PROJECTILE_COLLISION ECC_EngineTraceChannel1
#define CLIMBWALL_COLLISION ECC_EngineTraceChannel2
#define WEAPON_COLLISION ECC_EngineTraceChannel3
#define PLAYER_COLLISION ECC_EngineTraceChannel4
#define ENEMY_COLLISION ECC_GameTraceChannel5

void ASuraEnemyProjectile::BeginPlay()
{
	Super::BeginPlay();

	InitializeProjectile();
}

// Sets default values
ASuraEnemyProjectile::ASuraEnemyProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &ASuraEnemyProjectile::OnHit);

	// Prevent the player walking on the bullet
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	RootComponent = CollisionComp;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->bAutoActivate = false;
	ProjectileMovement->ProjectileGravityScale = 0.f;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(RootComponent);
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ProjectileMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	ProjectileMesh->SetCastShadow(false);

	ProjectileType = "Base";
}

void ASuraEnemyProjectile::InitializeProjectile()
{
	// CollisionComp->SetCollisionObjectType(PROJECTILE_COLLISION);
	// CollisionComp->SetCollisionResponseToChannel(PROJECTILE_COLLISION, ECR_Ignore); //Projectile
	// CollisionComp->SetCollisionResponseToChannel(WEAPON_COLLISION, ECR_Ignore); //Weapon
	// CollisionComp->SetCollisionResponseToChannel(ENEMY_COLLISION, ECR_Ignore); //Enemies
	
	if (const auto ProjectileAttributeData = EnemyProjectileAttributesDT.DataTable->FindRow<FEnemyProjectileAttributesData>(ProjectileType, ""))
	{
		ProjectileMovement->InitialSpeed = ProjectileAttributeData->InitialSpeed;
		ProjectileMovement->MaxSpeed = ProjectileAttributeData->MaxSpeed;

		M_DamageAmount = ProjectileAttributeData->DamageAmount;
		M_HeadshotAdditionalDamage = ProjectileAttributeData->HeadshotAdditionalDamage;
		M_LifeSapn = ProjectileAttributeData->LifeSpan;
		M_InitialRadius = ProjectileAttributeData->InitialRadius;
		M_ExplosionRadius = ProjectileAttributeData->ExplosionRadius;
		M_HomingAccelerationMagnitude = ProjectileAttributeData->HomingAccelerationMagnitude;
	}
	
	CollisionComp->SetSphereRadius(M_InitialRadius);

	CollisionComp->OnComponentHit.AddDynamic(this, &ASuraEnemyProjectile::OnHit);
}

void ASuraEnemyProjectile::SetOwner(AActor* TheOwner)
{
	Super::SetOwner(TheOwner);
}

void ASuraEnemyProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("hit"));

	if (OtherActor != nullptr && OtherActor != ProjectileOwner) 
	{
		if (OtherActor != this && OtherComp != nullptr && OtherComp->IsSimulatingPhysics())
			OtherComp->AddImpulseAtLocation(GetVelocity() * 100.f, GetActorLocation());

		ApplyDamage(OtherActor, M_DamageAmount, EDamageType::Projectile, false);

		Destroy();
	}
	/*else if (OtherActor != this && OtherComp != nullptr && OtherComp->IsSimulatingPhysics()) 
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.f, GetActorLocation());

		Destroy();
	}*/
}

void ASuraEnemyProjectile::ApplyDamage(AActor* OtherActor, float TheDamageAmount, EDamageType DamageType, bool bCanForceDamage)
{
	FDamageData Damage;
	Damage.DamageAmount = TheDamageAmount;
	Damage.DamageType = DamageType;
	Damage.bCanForceDamage = bCanForceDamage;

	if (OtherActor->GetClass()->ImplementsInterface(UDamageable::StaticClass()))
		Cast<IDamageable>(OtherActor)->TakeDamage(Damage, this->ProjectileOwner);

}

void ASuraEnemyProjectile::SetHomingTarget(const AActor* Target)
{
	ProjectileMovement->bIsHomingProjectile = true;
	ProjectileMovement->HomingAccelerationMagnitude = M_HomingAccelerationMagnitude;
	ProjectileMovement->HomingTargetComponent = Target->GetRootComponent();
}

void ASuraEnemyProjectile::LaunchProjectile()
{
	ProjectileMovement->Activate();
}