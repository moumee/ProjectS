// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Enumerations/EDamageType.h"
#include "SuraEnemyProjectile.generated.h"

UCLASS()
class SURAS_API ASuraEnemyProjectile : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile", meta = (AlowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(VisibleDefaultsOnly, Category = "Projectile")
	USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere)
	AActor* ProjectileOwner;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	float DamageAmount = 1.f;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	float InitialSpeed = 10000.f;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	float MaxSpeed = 10000.f;

	float LifeSapn = 10.f;
	float AdditionalDamage;
	float InitialRadius = 10.f;
	float HomingAccelerationMagnitude = 3000.f;

public:	
	// Sets default values for this actor's properties
	ASuraEnemyProjectile();

	void InitializeProjectile(AActor* TheOwner, float TheAdditionalDamage = 0.f, float AdditionalRadius = 0.f);

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void ApplyDamage(AActor* OtherActor, float TheDamageAmount, EDamageType DamageType, bool bCanForceDamage);

	void SetHomingTarget(bool bIsHoming, AActor* Target);

	void LaunchProjectile();
};
