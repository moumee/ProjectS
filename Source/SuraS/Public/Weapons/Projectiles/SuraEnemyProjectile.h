// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Enumerations/EDamageType.h"
#include "SuraEnemyProjectile.generated.h"

class UNiagaraComponent;

UCLASS()
class SURAS_API ASuraEnemyProjectile : public AActor
{
	GENERATED_BODY()

protected:
	FName ProjectileType;

	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile", meta = (AlowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(VisibleDefaultsOnly, Category = "Projectile")
	USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Niagara")
	UNiagaraComponent* NiagaraComponent;
	
	float M_DamageAmount;
	float M_HeadshotAdditionalDamage;
	float M_LifeSapn;
	float M_InitialRadius;
	float M_ExplosionRadius;
	float M_HomingAccelerationMagnitude;
	float M_DestroyDurationAfterLaunch;
	float M_DestroyDurationAfterHit;

public:	
	// Sets default values for this actor's properties
	ASuraEnemyProjectile();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (RowType = "/Script/SuraS.EnemyProjectileAttributesData"))
	FDataTableRowHandle EnemyProjectileAttributesDT;

	void InitializeProjectile();

	virtual void SetOwner(AActor* TheOwner) override;
	
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

	UNiagaraComponent* GetNiagaraComponent() const { return NiagaraComponent; }

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void ApplyDamage(AActor* OtherActor, float TheDamageAmount, EDamageType DamageType, bool bCanForceDamage);

	void SetHomingTarget(const AActor* Target);

	virtual void LaunchProjectile();

	virtual void LaunchProjectileWithVelocity(const FVector& Velocity);
};
