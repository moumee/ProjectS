// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/Projectiles/SuraEnemyProjectile.h"

#include "Interfaces/Damageable.h"
#include "Structures/DamageData.h"
#include "Characters/Player/SuraCharacterPlayer.h"

// Sets default values
ASuraEnemyProjectile::ASuraEnemyProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	//CollisionComp->SetCollisionObjectType(ECC_GameTraceChannel1);
	// CollisionComp->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore); //Projectile
	//CollisionComp->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Ignore); //Weapon
	// CollisionComp->SetCollisionResponseToChannel(ECC_GameTraceChannel5, ECR_Ignore); //Enemies
	CollisionComp->OnComponentHit.AddDynamic(this, &ASuraEnemyProjectile::OnHit);

	// Prevent the player walking on the bullet
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	RootComponent = CollisionComp;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = InitialSpeed;
	ProjectileMovement->MaxSpeed = MaxSpeed;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->bAutoActivate = false;
	ProjectileMovement->ProjectileGravityScale = 0.f;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(RootComponent);
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ProjectileMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
	ProjectileMesh->SetCastShadow(false);
}

void ASuraEnemyProjectile::InitializeProjectile(AActor* TheOwner, float TheAdditionalDamage, float AdditionalRadius)
{
	if (IsValid(TheOwner))
		ASuraEnemyProjectile::ProjectileOwner = TheOwner;

	ASuraEnemyProjectile::AdditionalDamage = TheAdditionalDamage;

	if (AdditionalRadius > 0.f)
		CollisionComp->SetSphereRadius(InitialRadius + AdditionalRadius);

	CollisionComp->OnComponentHit.AddDynamic(this, &ASuraEnemyProjectile::OnHit);
}

void ASuraEnemyProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("hit"));

	if (OtherActor != nullptr && OtherActor != ProjectileOwner) 
	{
		if (OtherActor != this && OtherComp != nullptr && OtherComp->IsSimulatingPhysics())
			OtherComp->AddImpulseAtLocation(GetVelocity() * 100.f, GetActorLocation());

		ApplyDamage(OtherActor, DamageAmount + AdditionalDamage, EDamageType::Melee, false);

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

void ASuraEnemyProjectile::SetHomingTarget(bool bIsHoming, AActor* Target)
{
	ProjectileMovement->bIsHomingProjectile = bIsHoming;
	ProjectileMovement->HomingAccelerationMagnitude = HomingAccelerationMagnitude;
	ProjectileMovement->HomingTargetComponent = Target->GetRootComponent();
}

void ASuraEnemyProjectile::LaunchProjectile()
{
	ProjectileMovement->Activate();
}