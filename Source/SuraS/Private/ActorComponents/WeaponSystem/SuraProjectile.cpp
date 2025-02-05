

#include "ActorComponents/WeaponSystem/SuraProjectile.h"

#include "Interfaces/Damageable.h"
#include "Structures/DamageData.h"
#include "Enumerations/EDamageType.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASuraProjectile::ASuraProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// PrimaryActorTick.bCanEverTick = true;

	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->SetCollisionObjectType(ECC_GameTraceChannel1);
	CollisionComp->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore); //Projectile
	CollisionComp->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Ignore); //Weapon
	CollisionComp->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	CollisionComp->OnComponentHit.AddDynamic(this, &ASuraProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	//TODO: Projectile attribute load �ϱ� ���� Data Table �����ϱ�
	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 500000.f;
	ProjectileMovement->MaxSpeed = 500000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.f;

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	ProjectileMesh->SetupAttachment(RootComponent);
	//ProjectileMesh->SetCollisionObjectType(ECC_GameTraceChannel1);
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//ProjectileMesh->SetCollisionResponseToAllChannels(ECR_Ignore);

	ProjectileMesh->SetCastShadow(false);

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void ASuraProjectile::InitializeProjectile(AActor* OwnerOfProjectile)
{
	if (IsValid(OwnerOfProjectile))
	{
		ProjectileOwner = OwnerOfProjectile;
	}
}

void ASuraProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//TODO: Projectile�� �ٸ� actor���� hit ���� ��, OtherActor�� ������ ���� �ٸ� event �߻���Ű��. Interface ����ϱ�
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
			SpawnParticleEffect(Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
			SpawnDecalEffect(Hit.ImpactPoint, Hit.ImpactNormal.Rotation());


			FDamageData DefaultDamage;
			DefaultDamage.DamageAmount = 50.f;
			DefaultDamage.DamageType = EDamageType::Melee;
			DefaultDamage.bCanForceDamage = false;

			if (OtherActor->GetClass()->ImplementsInterface(UDamageable::StaticClass()))
			{
				//UE_LOG(LogTemp, Warning, TEXT("Enemy Hit!!!"));
				Cast<IDamageable>(OtherActor)->TakeDamage(DefaultDamage, this->ProjectileOwner);
			}

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
		SpawnParticleEffect(Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
		SpawnDecalEffect(Hit.ImpactPoint, Hit.ImpactNormal.Rotation());


		Destroy();
	}
}

void ASuraProjectile::SpawnParticleEffect(FVector SpawnLocation, FRotator SpawnRotation)
{
	if (ImpactEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),       // ���� ���ؽ�Ʈ
			ImpactEffect,  // ��ƼŬ �ý��� ���ø�
			SpawnLocation,    // ��ġ
			SpawnRotation,    // ȸ��
			FVector(1.0f),    // ������
			true              // �ڵ� �ı� ����
		);
	}
}

void ASuraProjectile::SpawnDecalEffect(FVector SpawnLocation, FRotator SpawnRotation)
{
	if (DecalMaterial)
	{
		FVector DecalSize = FVector(32.0f, 64.0f, 64.0f);     // X: �β�, YZ: ũ��

		UGameplayStatics::SpawnDecalAtLocation(
			GetWorld(),          // ���� ���ؽ�Ʈ
			DecalMaterial,       // ��Į ��Ƽ����
			DecalSize,           // ��Į ũ��
			SpawnLocation,       // ��ġ
			SpawnRotation,       // ȸ��
			10.0f                // ���� (�� ����)
		);
	}
}

//// Called when the game starts or when spawned
//void ASuraProjectile::BeginPlay()
//{
//	Super::BeginPlay();
//	
//}
//
//// Called every frame
//void ASuraProjectile::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}


