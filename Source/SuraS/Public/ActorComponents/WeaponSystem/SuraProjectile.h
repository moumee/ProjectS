

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"

#include "Enumerations/EDamageType.h"

#include "ProjectileData.h"
#include "ProjectileType.h"

#include "SuraProjectile.generated.h"

class AWeapon;

class ASuraCharacterEnemyBase;

class USphereComponent;
class UProjectileMovementComponent;
class UNiagaraSystem;

DECLARE_DELEGATE(FHeadShotDelegate);
DECLARE_DELEGATE(FBodyShotDelegate);

UCLASS(config = Game) //TODO: ���� �Ӽ����� �˾ƺ�����
class SURAS_API ASuraProjectile : public AActor
{
	GENERATED_BODY()
public:
	FHeadShotDelegate OnHeadShot;
	FBodyShotDelegate OnBodyShot;
	// suhyeon
	const FProjectileData* GetProjectileData() const { return ProjectileData; }
protected:
	UPROPERTY(EditAnywhere, Category = Projectile)
	EProjectileType ProjectileType = EProjectileType::Projectile_Rifle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Projectile)
	FDataTableRowHandle ProjectileDataTableHandle;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (RowType="ProjectileData"))
	//FDataTableRowHandle ProjectileDataTableHandle;

	FProjectileData* ProjectileData;

	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	USphereComponent* CollisionComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;
	
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	UStaticMeshComponent* ProjectileMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UNiagaraSystem* TrailEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	UNiagaraSystem* ExplosionEffect;

	UPROPERTY()
	UNiagaraComponent* TrailEffectComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UNiagaraSystem* ImpactEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UMaterialInterface* DecalMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DefaultDamage = 0.f;

	float HeadShotAdditionalDamage = 0.f;

	UPROPERTY(VisibleAnywhere)
	AActor* ProjectileOwner;

	UPROPERTY(VisibleAnywhere)
	AWeapon* Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CustomProjectile")
	float InitialSpeed = 50000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CustomProjectile")
	float MaxSpeed = 50000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CustomProjectile")
	float InitialRadius = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CustomProjectile")
	float ProjectileRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosive")
	bool bIsExplosive = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosive")
	bool bVisualizeExplosionRadius = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosive")
	float MaxExplosiveDamage = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Explosive")
	float MaxExplosionRadius = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CustomProjectile")
	float HomingAccelerationMagnitude = 3000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Penetration")
	bool bCanPenetrate = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Penetration")
	int32 NumPenetrableObjects = 4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ricochet")
	bool bCanSimpleBounce = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ricochet")
	int32 MaxRicochetCount = 4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ricochet")
	float MinIncidenceAngle = 5.f;

	int32 CurrentRicochetCount = 0;


public:	
	ASuraProjectile();
	void InitializeProjectile(AActor* Owner, AWeapon* OwnerWeapon, float additonalDamage = 0.f, float AdditionalRadius = 0.f, int32 NumPenetrable = 0, bool HitScan = false);
	void LoadProjectileData();
	void SetHomingTarget(bool bIsHoming, AActor* Target);
	void LaunchProjectile();

	void ApplyExplosiveDamage(bool bCanExplosiveDamage, FVector CenterLocation);
	void ApplyDamage(AActor* OtherActor, float DamageAmount, EDamageType DamageType, bool bCanForceDamage, FName BoneName);
	bool SearchOverlappedActor(FVector CenterLocation, float SearchRadius, TArray<AActor*>& OverlappedActors);

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	USphereComponent* GetCollisionComp() const { return CollisionComp; }
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

	void SpawnImpactEffect(FVector SpawnLocation, FRotator SpawnRotation);
	void SpawnExplosionEffect(FVector SpawnLocation);
	void SpawnTrailEffect(bool bShouldAttachedToWeapon = false);
	void SpawnDecalEffect(FVector SpawnLocation, FRotator SpawnRotation);
protected:
	bool bShouldUpdateTrailEffect = false;
	void UpdateTrailEffect();


	void DrawSphere(FVector Location, float Radius);

//protected:
//	// Called when the game starts or when spawned
//	virtual void BeginPlay() override;
//
public:	
	virtual void Tick(float DeltaTime) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void BeginDestroy() override;

#pragma region Sound
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* HitSound_Default;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* HitSound_Metal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* HitSound_Grass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* HitSound_Enemy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* HitSound_Energy;
	void PlaySoundAtLocationByMaterial(EPhysicalSurface SurfaceType, FVector Location);
#pragma endregion

#pragma region HitScan
protected:
	bool bDebugHitScan = false;
	bool bIsHitScan = false;
	bool bActivatedMeshMovementForHitScan = false;
	TArray<FVector> HitScanEndPoints;
	int32 CurrEndPointIdx = 0;
	float HitScanProjectileVelocity;
	float DistanceMoved;
	float TargetDistance;
	FVector MovementDirection;
	void PerformHitScan(FVector StartLocation, FVector TraceDirection, float MaxDistance, float SphereRadius, TArray<FVector>& OutHitLocations);
	void InitHitScanProjectileMovement();
	void UpdateHitScanProjectileMovement(float DeltaTime);
public:
	void SetHitScanActive(bool bflag);
	void LaunchHitScan(FVector StartLocation, FVector TraceDirection);

#pragma endregion

#pragma region Penetration
protected:
	int32 NumPenetratedObjects = 0;

	float AdditionalDamage = 0.f;
protected:
	void UpdatePenetration();
	void ResetPenetration();
#pragma endregion

#pragma region HeadShot
protected:
	bool CheckHeadHit(const FHitResult& Hit);
	bool CheckHeadOvelap(const AActor* OverlappedActor, const FHitResult& SweepResult);
#pragma endregion

#pragma region Homing
protected:
	//TODO: DTȭ ��Ű��
	float ExlosionTriggerRadius = 10.f;

	UPROPERTY()
	ASuraCharacterEnemyBase* TargetEnemy = nullptr;
	FVector RecentTargetLocation;
protected:
	bool IsTargetValid();
	bool IsTargetWithInRange();
	void UpdateTargetInfo();
#pragma endregion

#pragma region Impulse
protected:
	bool bCanApplyImpulseToEnemy = false;
	float HitImpulseToEnemy = 100.f;
protected:
	void AddImpulseToEnemy(AActor* OtherActor, FVector Force);
#pragma endregion

#pragma region Ricochet
protected:
	bool CheckRicochetAngle(FVector normal, FVector vel);
	FVector GetReflectionAngle(FVector normal, FVector input);
#pragma endregion;

};
