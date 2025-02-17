

#include "ActorComponents/WeaponSystem/SuraProjectile.h"

#include "ActorComponents/WeaponSystem/ACWeapon.h"

#include "Interfaces/Damageable.h"
#include "Structures/DamageData.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"
//#include "Particles/ParticleSystem.h"
//#include "Particles/ParticleSystemComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
ASuraProjectile::ASuraProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	//CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->SetCollisionObjectType(ECC_GameTraceChannel1);
	CollisionComp->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore); //Projectile
	CollisionComp->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Ignore); //Weapon
	CollisionComp->SetCollisionResponseToChannel(ECC_GameTraceChannel4, ECR_Ignore); //Player
	CollisionComp->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	//CollisionComp->OnComponentHit.AddDynamic(this, &ASuraProjectile::OnHit);		// set up a notification for when this component hits something blocking
	//CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ASuraProjectile::OnComponentBeginOverlap);


	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	//TODO: Projectile attribute load �ϱ� ���� Data Table �����ϱ�
	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = InitialSpeed;
	ProjectileMovement->MaxSpeed = MaxSpeed;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.f;
	ProjectileMovement->bAutoActivate = false;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(RootComponent);
	//ProjectileMesh->SetCollisionObjectType(ECC_GameTraceChannel1);
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ProjectileMesh->SetCollisionResponseToAllChannels(ECR_Ignore);

	ProjectileMesh->SetCastShadow(false);

	InitialLifeSpan = 10.0f;

	UE_LOG(LogTemp, Warning, TEXT("Projectile is Spawned!!!"));
}

void ASuraProjectile::InitializeProjectile(AActor* OwnerOfProjectile, UACWeapon* OwnerWeapon, float additonalDamage, float AdditionalRadius) //TODO: ���⼭ ProjectileType�� input���� �޾ƾ� �� ��
{
	// Weapon���� spawn projectile �� �� ó���� ����� �ϳ�?
	// �ٵ� ������ projectile �������� BP ���� �����, Mesh�� �ٸ��� ����ϴµ� �ǹ̰� �ֳ�?

	// ���: �Ʒ�ó�� ó������ Projectile�� type�� ���س��� �׿� �´� DT low�� Ž���ϴ� ���� �ƴ϶�,
	// BP���� DT RowBase�� ���� �����ϰ� �ϰ�, ���õ� DT RowBase�� ���� �Ӽ������� �ҷ����� ������ �ϱ�(Projectile�� Type Enum ���� ����������)

	if (IsValid(OwnerWeapon))
	{
		Weapon = OwnerWeapon;
	}

	if (IsValid(OwnerOfProjectile))
	{
		ProjectileOwner = OwnerOfProjectile;

		if (ProjectileType == EProjectileType::Projectile_Rifle)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Rifle Projectile!!!"));
			LoadProjectileData("RifleProjectile");
			SpawnTrailEffect();
		}
		else if (ProjectileType == EProjectileType::Projectile_ShotGun)
		{
			//UE_LOG(LogTemp, Warning, TEXT("ShotGun Projectile!!!"));
			LoadProjectileData("ShotGunProjectile");
			SpawnTrailEffect();
		}
		else if (ProjectileType == EProjectileType::Projectile_BasicRocket)
		{
			//UE_LOG(LogTemp, Warning, TEXT("BasicRocket Projectile!!!"));
			LoadProjectileData("BasicRocketProjectile");
			SpawnTrailEffect();
		}
		else if (ProjectileType == EProjectileType::Projectile_RailGun)
		{
			LoadProjectileData("RailGunProjectile");
			SpawnTrailEffect(true);
		}
	}


	if (bCanPenetrate)
	{
		CollisionComp->OnComponentHit.AddDynamic(this, &ASuraProjectile::OnHit);
		CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ASuraProjectile::OnComponentBeginOverlap);

		CollisionComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	}
	else
	{
		CollisionComp->OnComponentHit.AddDynamic(this, &ASuraProjectile::OnHit);
	}

	AdditionalDamage = additonalDamage;

	if (AdditionalRadius > 0.f)
	{
		CollisionComp->SetSphereRadius(InitialRadius + AdditionalRadius);
	}

	UE_LOG(LogTemp, Warning, TEXT("Projectile InitialSpeed: %f"), ProjectileMovement->InitialSpeed);
	UE_LOG(LogTemp, Warning, TEXT("Projectile MaxSpeed: %f"), ProjectileMovement->MaxSpeed);
}

void ASuraProjectile::LoadProjectileData(FName ProjectileID)
{
	ProjectileData = ProjectileDataTable->FindRow<FProjectileData>(ProjectileID, TEXT(""));
	if (ProjectileData)
	{
		TrailEffect = ProjectileData->TrailEffect;
		ImpactEffect = ProjectileData->ImpactEffect;
		DecalMaterial = ProjectileData->HoleDecal;

		InitialLifeSpan = ProjectileData->InitialLifeSpan; //TODO �̷��Դ� ������ �ȵ�. ���� ���
		SetLifeSpan(ProjectileData->InitialLifeSpan);

		DefaultDamage = ProjectileData->Damage;
		bIsExplosive = ProjectileData->bIsExplosive;
		MaxExplosiveDamage = ProjectileData->MaxExplosiveDamage;
		MaxExplosionRadius = ProjectileData->MaxExplosionRadius;

		HomingAccelerationMagnitude = ProjectileData->HomingAccelerationMagnitude;

		ProjectileMovement->InitialSpeed = ProjectileData->InitialSpeed;
		ProjectileMovement->MaxSpeed = ProjectileData->MaxSpeed;

		InitialRadius = ProjectileData->InitialRadius;
		CollisionComp->SetSphereRadius(InitialRadius);

		bCanPenetrate = ProjectileData->bCanPenetrate;
		NumPenetrableObjects = ProjectileData->NumPenetrableObjects;
	}
}

void ASuraProjectile::SetHomingTarget(bool bIsHoming, AActor* Target)
{
	ProjectileMovement->bIsHomingProjectile = bIsHoming;
	ProjectileMovement->HomingAccelerationMagnitude = HomingAccelerationMagnitude;
	ProjectileMovement->HomingTargetComponent = Target->GetRootComponent();

	UE_LOG(LogTemp, Warning, TEXT("Homing!!!"));
}

void ASuraProjectile::LaunchProjectile()
{
	ProjectileMovement->Activate();

	if (ProjectileMovement->bIsHomingProjectile)
	{
		UE_LOG(LogTemp, Error, TEXT("Is Homing!"));
	}
}

void ASuraProjectile::ApplyExplosiveDamage(bool bCanExplosiveDamage, FVector CenterLocation)
{
	if (bCanExplosiveDamage)
	{
		TArray<AActor*> OverlappedActors;
		if (SearchOverlappedActor(CenterLocation, MaxExplosionRadius, OverlappedActors))
		{
			for (AActor* OverlappedActor : OverlappedActors)
			{
				float DistanceToTarget = FVector::Distance(CenterLocation, OverlappedActor->GetActorLocation());
				float DamageAmount;
				if (DistanceToTarget > MaxExplosionRadius)
				{
					DamageAmount = 0.f;
				}
				else
				{
					DamageAmount = ((MaxExplosionRadius - DistanceToTarget) / MaxExplosionRadius) * MaxExplosiveDamage;
				}
				ApplyDamage(OverlappedActor, DamageAmount, EDamageType::Explosion, true);
				UE_LOG(LogTemp, Error, TEXT("Explosive Damage!!!"));
			}
		}
	}
}
void ASuraProjectile::ApplyDamage(AActor* OtherActor, float DamageAmount, EDamageType DamageType, bool bCanForceDamage)
{
	FDamageData Damage;
	Damage.DamageAmount = DamageAmount;
	Damage.DamageType = DamageType;
	Damage.bCanForceDamage = bCanForceDamage;

	if (OtherActor->GetClass()->ImplementsInterface(UDamageable::StaticClass()))
	{
		Cast<IDamageable>(OtherActor)->TakeDamage(Damage, this->ProjectileOwner);
	}
}
bool ASuraProjectile::SearchOverlappedActor(FVector CenterLocation, float SearchRadius, TArray<AActor*>& OverlappedActors)
{
	TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes;
	traceObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	TArray<AActor*> ignoreActors;
	ignoreActors.Init(ProjectileOwner, 1);
	bool bIsAnyActorExist = UKismetSystemLibrary::SphereOverlapActors(GetWorld(), CenterLocation, SearchRadius, traceObjectTypes, nullptr, ignoreActors, OverlappedActors);

	return bIsAnyActorExist;
}

void ASuraProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//TODO: Projectile�� �ٸ� actor���� hit ���� ��, OtherActor�� ������ ���� �ٸ� event �߻���Ű��. Interface ����ϱ�
	if (bCanPenetrate)
	{
		SpawnImpactEffect(Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
		SpawnDecalEffect(Hit.ImpactPoint, Hit.ImpactNormal.Rotation());

		if (bShouldUpdateTrailEffect)
		{
			if (TrailEffectComponent)
			{
				TrailEffectComponent->Deactivate();
				TrailEffectComponent->DestroyComponent();
				TrailEffectComponent = nullptr;
			}
		}

		Destroy();
	}
	else
	{
		if (OtherActor != nullptr)
		{
			if (OtherActor != ProjectileOwner)
			{
				// Only add impulse and destroy projectile if we hit a physics
				if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
				{
					OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
				}

				//TODO: Effect�� �ѹ��� �߻��ǵ��� �����ϱ�
				//TODO: �浹 Effect�� Projectile Type�� ���� �ٸ��� �����ϱ�
				SpawnImpactEffect(Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
				SpawnDecalEffect(Hit.ImpactPoint, Hit.ImpactNormal.Rotation());

				ApplyDamage(OtherActor, DefaultDamage + AdditionalDamage, EDamageType::Melee, false);
				ApplyExplosiveDamage(bIsExplosive, Hit.ImpactPoint);

				Destroy();
			}
		}
		else
		{
			// Only add impulse and destroy projectile if we hit a physics
			if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
			{
				OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
			}

			//TODO: Effect�� �ѹ��� �߻��ǵ��� �����ϱ�
			//TODO: �浹 Effect�� Projectile Type�� ���� �ٸ��� �����ϱ�
			SpawnImpactEffect(Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
			SpawnDecalEffect(Hit.ImpactPoint, Hit.ImpactNormal.Rotation());

			Destroy();
		}
	}
}

void ASuraProjectile::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Error, TEXT("BeginOverlapped!!!"));

	if (bCanPenetrate)
	{
		if (OtherActor != nullptr)
		{
			if (OtherActor != ProjectileOwner)
			{
				if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
				{
					OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
				}

				//TODO: Overlap �� ���� Effect�� ��� ���� �����غ�����
				//SpawnImpactEffect(Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
				//SpawnDecalEffect(Hit.ImpactPoint, Hit.ImpactNormal.Rotation());

				//TODO: Damage�� Initialize���� ���� �޾ƿ;���. Additional Damage�� �޾ƿ;���
				//�ϴ��� �⺻ Damage�� ����
				ApplyDamage(OtherActor, DefaultDamage + AdditionalDamage, EDamageType::Melee, false);
				ApplyExplosiveDamage(bIsExplosive, SweepResult.ImpactPoint);

				UpdatePenetration();
				if (NumPenetratedObjects >= NumPenetrableObjects)
				{
					ResetPenetration();


					if (bShouldUpdateTrailEffect)
					{
						TrailEffectComponent->Deactivate();
						TrailEffectComponent->DestroyComponent();
					}


					Destroy();
				}
			}
		}
	}
}


void ASuraProjectile::SpawnImpactEffect(FVector SpawnLocation, FRotator SpawnRotation)
{
	if (ImpactEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ImpactEffect, SpawnLocation, SpawnRotation, FVector(1.0f), true);
	}
}
void ASuraProjectile::SpawnTrailEffect(bool bShouldAttachedToWeapon) //TODO: Rocket Trail ����� �̻���. �պ�����
{
	if (ProjectileMesh && TrailEffect)
	{
		FTransform TrailStartTransform = ProjectileMesh->GetSocketTransform(FName(TEXT("TrailStart")), ERelativeTransformSpace::RTS_Component);
		FTransform TrailEndTransform = ProjectileMesh->GetSocketTransform(FName(TEXT("TrailEnd")), ERelativeTransformSpace::RTS_Component);

		// TODO: offset�� Traile ���� �ٸ��� �����ؾ���. �ƴϸ� �����Ϳ��� Mesh���� socket�� ��ġ�� ���� �ٲ��ִ� ���� ���� ���������
		float DistanceOffset = 80.f;

		FVector TrailLocationOffset = (TrailEndTransform.GetLocation() - TrailStartTransform.GetLocation()).GetSafeNormal() * DistanceOffset;
		
		if (bShouldAttachedToWeapon)
		{
			UE_LOG(LogTemp, Error, TEXT("Spawn Trail Effect!!!"));


			TrailEffectComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				GetWorld(),
				TrailEffect,
				Weapon->GetSocketLocation(FName(TEXT("Muzzle"))),
				FRotator(0.f, 0.f, 0.f));


			//TODO: effect�� weapon muzzle�� ������ų��, �߻������� �������� Input���� ���������ϰ� �ϱ�
			
			//TrailEffectComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
			//	TrailEffect,
			//	Weapon,
			//	FName(TEXT("Muzzle")),
			//	FVector(0, 0, 0),
			//	FRotator(0, 0, 0),
			//	EAttachLocation::KeepRelativeOffset,
			//	true);

			bShouldUpdateTrailEffect = true;


			TrailEffectComponent->SetVectorParameter(FName(TEXT("Beam End")), ProjectileMesh->GetSocketLocation(FName(TEXT("TrailStart"))));
		}
		else
		{
			TrailEffectComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
				TrailEffect,
				ProjectileMesh,
				FName(TEXT("TrailStart")),
				TrailLocationOffset,
				FRotator(0, 0, 0),
				EAttachLocation::KeepRelativeOffset,
				true);
		}
	}
}
void ASuraProjectile::SpawnDecalEffect(FVector SpawnLocation, FRotator SpawnRotation)
{
	if (DecalMaterial)
	{
		FVector DecalSize = FVector(2.0f, 8.0f, 8.0f);     // X: �β�, YZ: ũ��

		UDecalComponent* ProjectileDecal =
		UGameplayStatics::SpawnDecalAtLocation(
			GetWorld(),          // ���� ���ؽ�Ʈ
			DecalMaterial,       // ��Į ��Ƽ����
			DecalSize,           // ��Į ũ��
			SpawnLocation,       // ��ġ
			SpawnRotation,       // ȸ��
			10.0f                // ���� (�� ����)
		);
		ProjectileDecal->SetFadeScreenSize(0.0001f);
	}
}

void ASuraProjectile::UpdateTrailEffect()
{
	if (bShouldUpdateTrailEffect)
	{
		if (TrailEffectComponent)
		{
			TrailEffectComponent->SetVectorParameter(FName(TEXT("Beam End")), ProjectileMesh->GetSocketLocation(FName(TEXT("TrailStart"))));
		}
	}
}

#pragma region Penetration
void ASuraProjectile::UpdatePenetration() //TODO: ���� �Լ��� �߾�� �߳�?
{
	NumPenetratedObjects++;
}

void ASuraProjectile::ResetPenetration()  //TODO: ���� �Լ��� �߾�� �߳�?
{
	NumPenetratedObjects = 0;
}
#pragma endregion

//// Called when the game starts or when spawned
//void ASuraProjectile::BeginPlay()
//{
//	Super::BeginPlay();
//	
//}
//
void ASuraProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateTrailEffect();
}

void ASuraProjectile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (TrailEffectComponent)
	{
		if (bShouldUpdateTrailEffect)
		{
			TrailEffectComponent->Deactivate();
			TrailEffectComponent->DestroyComponent();
		}
	}
}

void ASuraProjectile::BeginDestroy()
{
	Super::BeginDestroy();
}


