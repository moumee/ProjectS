
#include "ActorComponents/WeaponSystem/SuraProjectile.h"
#include "ActorComponents/WeaponSystem/ACWeapon.h"

#include "Characters/Enemies/SuraCharacterEnemyBase.h"
#include "Interfaces/Damageable.h"
#include "Structures/DamageData.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"

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
	CollisionComp->SetCollisionObjectType(ECC_GameTraceChannel7);
	CollisionComp->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Ignore); //Projectile
	CollisionComp->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Ignore); //Weapon
	CollisionComp->SetCollisionResponseToChannel(ECC_GameTraceChannel4, ECR_Ignore); //Player
	CollisionComp->SetCollisionResponseToChannel(ECC_GameTraceChannel7, ECR_Ignore); //PlayerProjectile
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

void ASuraProjectile::InitializeProjectile(AActor* OwnerOfProjectile, AWeapon* OwnerWeapon, float additonalDamage, float AdditionalRadius, int32 NumPenetrable, bool HitScan)
{
	if (IsValid(OwnerWeapon))
	{
		Weapon = OwnerWeapon;
	}

	if (IsValid(OwnerOfProjectile))
	{
		ProjectileOwner = OwnerOfProjectile;

		LoadProjectileData();
		SpawnTrailEffect();
	}

	if (HitScan)
	{
		bIsHitScan = HitScan;
		NumPenetrableObjects = NumPenetrable;
		UE_LOG(LogTemp, Error, TEXT("Projectile Penetrable Num: %d"), NumPenetrableObjects);
	}
	else
	{
		if (NumPenetrable > 0 || bCanPenetrate)
		{
			CollisionComp->OnComponentHit.AddDynamic(this, &ASuraProjectile::OnHit);
			CollisionComp->OnComponentBeginOverlap.AddDynamic(this, &ASuraProjectile::OnComponentBeginOverlap);
			CollisionComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
			CollisionComp->SetCollisionResponseToChannel(ECC_GameTraceChannel6, ECR_Overlap);
			NumPenetrableObjects = NumPenetrable;
			UE_LOG(LogTemp, Error, TEXT("Projectile Penetrable Num: %d"), NumPenetrableObjects);
		}
		else
		{
			CollisionComp->OnComponentHit.AddDynamic(this, &ASuraProjectile::OnHit);
		}
	}

	if (bCanSimpleBounce)
	{
		// MEMO: Test
		ProjectileMovement->bShouldBounce = true; 
		ProjectileMovement->Bounciness = 0.6f;    //(0~1)
		ProjectileMovement->Friction = 0.2f;     
		ProjectileMovement->BounceVelocityStopSimulatingThreshold = 10.0f;
		ProjectileMovement->bRotationFollowsVelocity = true;
	}


	AdditionalDamage = additonalDamage;

	if (AdditionalRadius > 0.f)
	{
		ProjectileRadius = InitialRadius + AdditionalRadius;
		CollisionComp->SetSphereRadius(InitialRadius + AdditionalRadius);
	}

	UE_LOG(LogTemp, Warning, TEXT("Projectile InitialSpeed: %f"), ProjectileMovement->InitialSpeed);
	UE_LOG(LogTemp, Warning, TEXT("Projectile MaxSpeed: %f"), ProjectileMovement->MaxSpeed);
}

void ASuraProjectile::LoadProjectileData()
{
	//ProjectileData = ProjectileDataTable->FindRow<FProjectileData>(ProjectileID, TEXT(""));
	ProjectileData = ProjectileDataTableHandle.GetRow<FProjectileData>("");
	if (ProjectileData)
	{
		// <Effect>
		TrailEffect = ProjectileData->TrailEffect;
		ImpactEffect = ProjectileData->ImpactEffect;
		ExplosionEffect = ProjectileData->ExplosionEffect;
		DecalMaterial = ProjectileData->HoleDecal;

		InitialLifeSpan = ProjectileData->InitialLifeSpan; //TODO �̷��Դ� ������ �ȵ�. ���� ���
		SetLifeSpan(ProjectileData->InitialLifeSpan);

		// <Sound>
		HitSound = ProjectileData->HitSound;

		// <Damage>
		DefaultDamage = ProjectileData->DefaultDamage;
		HeadShotAdditionalDamage = ProjectileData->HeadShotAdditionalDamage;

		// <Explosive>
		bIsExplosive = ProjectileData->bIsExplosive;
		bVisualizeExplosionRadius = ProjectileData->bVisualizeExplosionRadius;
		MaxExplosiveDamage = ProjectileData->MaxExplosiveDamage;
		MaxExplosionRadius = ProjectileData->MaxExplosionRadius;

		// <Homing>
		HomingAccelerationMagnitude = ProjectileData->HomingAccelerationMagnitude;

		ProjectileMovement->InitialSpeed = ProjectileData->InitialSpeed;
		ProjectileMovement->MaxSpeed = ProjectileData->MaxSpeed;

		InitialRadius = ProjectileData->InitialRadius;
		CollisionComp->SetSphereRadius(InitialRadius);

		// <Penetration>
		bCanPenetrate = ProjectileData->bCanPenetrate; //legacy

		// <Impulse>
		bCanApplyImpulseToEnemy = ProjectileData->bCanApplyImpulseToEnemy;
		HitImpulseToEnemy = ProjectileData->HitImpulseToEnemy;

		// <Ricochet>
		bCanSimpleBounce = ProjectileData->bCanSimpleBounce;
		MaxRicochetCount = ProjectileData->MaxRicochetCount;
		MinIncidenceAngle = ProjectileData->MinIncidenceAngle;
	}
}

void ASuraProjectile::SetHomingTarget(bool bIsHoming, AActor* Target)
{
	ProjectileMovement->bIsHomingProjectile = bIsHoming;

	if (bIsHoming)
	{
		ProjectileMovement->HomingAccelerationMagnitude = HomingAccelerationMagnitude;
		ProjectileMovement->HomingTargetComponent = Target->GetRootComponent();

		TargetEnemy = Cast<ASuraCharacterEnemyBase>(Target);
		if (TargetEnemy)
		{
			RecentTargetLocation = TargetEnemy->GetActorLocation();
		}
		//UE_LOG(LogTemp, Warning, TEXT("Homing!!!"));
	}
}

void ASuraProjectile::LaunchProjectile()
{
	ProjectileMovement->Activate();
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
				ApplyDamage(OverlappedActor, DamageAmount, EDamageType::Explosion, true, NAME_None);
				if (OnBodyShot.IsBound())
				{
					OnBodyShot.Execute();
				}
				//UE_LOG(LogTemp, Error, TEXT("Explosive Damage!!!"));
			}
		}

		if (bVisualizeExplosionRadius)
		{
			DrawSphere(CenterLocation, MaxExplosionRadius);
		}
	}
}
void ASuraProjectile::ApplyDamage(AActor* OtherActor, float DamageAmount, EDamageType DamageType, bool bCanForceDamage, FName BoneName)
{
	FDamageData Damage;
	Damage.DamageAmount = DamageAmount;
	Damage.DamageType = DamageType;
	Damage.bCanForceDamage = bCanForceDamage;
	Damage.BoneName = BoneName;

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

				SpawnImpactEffect(Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
				SpawnDecalEffect(Hit.ImpactPoint, Hit.ImpactNormal.Rotation());

				if (HitSound != nullptr)
				{
					UGameplayStatics::PlaySoundAtLocation(this, HitSound, Hit.ImpactPoint);
				}

				if (HeadShotAdditionalDamage > 0.f && CheckHeadHit(Hit))
				{
					ApplyDamage(OtherActor, DefaultDamage + AdditionalDamage + HeadShotAdditionalDamage, EDamageType::Melee, false, Hit.BoneName);

					if (OnHeadShot.IsBound())
					{
						OnHeadShot.Execute();
					}
				}
				else
				{
					ApplyDamage(OtherActor, DefaultDamage + AdditionalDamage, EDamageType::Melee, false, Hit.BoneName);
					UE_LOG(LogTemp, Error, TEXT("bone11: %s"), *Hit.BoneName.ToString());
					if (Cast<ACharacter>(OtherActor))
					{
						if (OnBodyShot.IsBound())
						{
							OnBodyShot.Execute();
						}
					}
				}

				ApplyExplosiveDamage(bIsExplosive, Hit.ImpactPoint);

				if (bCanApplyImpulseToEnemy)
				{
					AddImpulseToEnemy(OtherActor, GetVelocity().GetSafeNormal()*HitImpulseToEnemy);
				}

				if (Cast<ACharacter>(OtherActor))
				{
					Destroy();
				}
				else
				{
					if (bCanSimpleBounce && CurrentRicochetCount < MaxRicochetCount && CheckRicochetAngle(Hit.ImpactNormal, ProjectileMovement->Velocity))
					{
						CurrentRicochetCount++;
					}
					else
					{
						Destroy();
					}
				}
			}
		}
		else
		{
			// Only add impulse and destroy projectile if we hit a physics
			if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
			{
				OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());
			}

			SpawnImpactEffect(Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
			SpawnDecalEffect(Hit.ImpactPoint, Hit.ImpactNormal.Rotation());

			Destroy();
		}
	}
}

void ASuraProjectile::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Error, TEXT("Projectile Overlapped!!!"));

	if (NumPenetrableObjects > 0 || bCanPenetrate)
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
				
				if (HeadShotAdditionalDamage > 0.f && CheckHeadOvelap(OtherActor, SweepResult))
				{
					ApplyDamage(OtherActor, DefaultDamage + AdditionalDamage + HeadShotAdditionalDamage, EDamageType::Melee, false, SweepResult.BoneName);

					if (OnHeadShot.IsBound())
					{
						OnHeadShot.Execute();
					}
				}
				else
				{
					ApplyDamage(OtherActor, DefaultDamage + AdditionalDamage, EDamageType::Melee, false, SweepResult.BoneName);

					UE_LOG(LogTemp, Error, TEXT("Projectile Overlapped!!!"));

					if (Cast<ACharacter>(OtherActor))
					{
						if (OnBodyShot.IsBound())
						{
							OnBodyShot.Execute();
						}
					}
				}

				ApplyExplosiveDamage(bIsExplosive, SweepResult.ImpactPoint);

				if (bCanApplyImpulseToEnemy)
				{
					AddImpulseToEnemy(OtherActor, GetVelocity().GetSafeNormal() * HitImpulseToEnemy);
				}

				UpdatePenetration();
				if (NumPenetratedObjects > NumPenetrableObjects)
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
void ASuraProjectile::SpawnExplosionEffect(FVector SpawnLocation)
{
	if (ExplosionEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ExplosionEffect, SpawnLocation, FRotator::ZeroRotator, FVector(1.0f), true);
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
				Weapon->GetWeaponMesh()->GetSocketLocation(FName(TEXT("Muzzle"))),
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



void ASuraProjectile::DrawSphere(FVector Location, float Radius)
{
	DrawDebugSphere(
		GetWorld(),                 // UWorld* World
		Location,         // ��ġ (Center)
		Radius,                     // ������ (Radius)
		12,                        // ���׸�Ʈ �� (Segments)
		FColor::Red,               // ����
		false,                     // ���� �ð� ���� (true�� ���������� ǥ�õ�)
		5.0f,                      // ���� �ð� (��)
		0,                         // Depth Priority
		2.0f                       // �� �β�
	);
}

#pragma region HitScan
void ASuraProjectile::SetHitScanActive(bool bflag)
{
	bIsHitScan = bflag;
}
void ASuraProjectile::LaunchHitScan(FVector StartLocation, FVector TraceDirection)
{
	PerformHitScan(StartLocation, TraceDirection, 50000.f, ProjectileRadius, HitScanEndPoints); //TODO: MaxDistnace 설정해야함

	bActivatedMeshMovementForHitScan = true;

}
void ASuraProjectile::PerformHitScan(FVector StartLocation, FVector TraceDirection, float MaxDistance, float SphereRadius, TArray<FVector>& OutHitLocations)
{
	FVector Start = StartLocation;
	FVector Direction = TraceDirection;
	FVector End = StartLocation + TraceDirection * MaxDistance;

	//TArray<FHitResult> HitResults;

	TArray<FVector> HitStaticLocations;

	FCollisionObjectQueryParams ObjectQueryParams;
	//ObjectQueryParams.AddObjectTypesToQuery(ECC_Visibility);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_PhysicsBody);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_GameTraceChannel6);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(ProjectileOwner);
	Params.AddIgnoredComponent(Weapon->GetWeaponMesh());
	Params.AddIgnoredComponent(ProjectileMesh);
	Params.AddIgnoredActor(this);


	//-------------------------------
	for (int32 RicochetCount = 0; RicochetCount <= MaxRicochetCount; RicochetCount++)
	{
		UE_LOG(LogTemp, Warning, TEXT("reflection"));

		TArray<FHitResult> TempHitResults;

		bool bHit = GetWorld()->SweepMultiByObjectType(
			TempHitResults,
			Start,
			End,
			FQuat::Identity,
			ObjectQueryParams,
			FCollisionShape::MakeSphere(SphereRadius),
			Params
		);
		
		DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 10.f);

		bool bIsBlockedByWorldStatic = false;

		if (bHit)
		{
			TArray<AActor*> OnceDamagedEnemies;
			for (const FHitResult& HitResult : TempHitResults)
			{
				// Damage
				if (NumPenetratedObjects <= NumPenetrableObjects)
				{
					ACharacter* Enemy = Cast<ACharacter>(HitResult.GetActor());
					if (Enemy && !OnceDamagedEnemies.Contains(Enemy))
					{
						OnceDamagedEnemies.AddUnique(Enemy);

						if (HeadShotAdditionalDamage > 0.f && CheckHeadHit(HitResult))
						{
							ApplyDamage(HitResult.GetActor(), DefaultDamage + AdditionalDamage + HeadShotAdditionalDamage, EDamageType::Melee, false, HitResult.BoneName);

							if (OnHeadShot.IsBound())
							{
								OnHeadShot.Execute();
							}
						}
						else
						{
							ApplyDamage(HitResult.GetActor(), DefaultDamage + AdditionalDamage, EDamageType::Melee, false, HitResult.BoneName);
							UE_LOG(LogTemp, Error, TEXT("bone11: %s"), *HitResult.BoneName.ToString());
							if (OnBodyShot.IsBound())
							{
								OnBodyShot.Execute();
							}
						}

						UpdatePenetration();
					}
				}


				//WorldStatic
				if (HitResult.GetComponent()->GetCollisionObjectType() == ECC_WorldStatic)
				{
					UE_LOG(LogTemp, Warning, TEXT("ECC_WorldStatic"));

					HitStaticLocations.Add(HitResult.ImpactPoint);
					DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 20.f, 12, FColor::Red, false, 50.f);

					Start = HitResult.ImpactPoint;

					if (CheckRicochetAngle(HitResult.ImpactNormal, Direction))
					{
						UE_LOG(LogTemp, Warning, TEXT("CheckRicochetAngle"));

						Direction = GetReflectionAngle(HitResult.ImpactNormal, Direction);
						Start = Start + Direction.GetSafeNormal() * (SphereRadius + 1.f);
						End = Start + Direction * MaxDistance;
						CurrentRicochetCount++;
					}
					else
					{
						RicochetCount = MaxRicochetCount + 1;
					}
					bIsBlockedByWorldStatic = true;
					break;
				}
			}
		}

		if (!bHit || !bIsBlockedByWorldStatic)
		{
			HitStaticLocations.Add(End);
			break;
		}		
	}

	OutHitLocations = HitStaticLocations;
}
void ASuraProjectile::UpdateHitScanProjectileMovement(float DeltaTime)
{

}
#pragma endregion

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

#pragma region HeadShot
bool ASuraProjectile::CheckHeadHit(const FHitResult& HitResult)
{
	//UE_LOG(LogTemp, Error, TEXT("FName: %s"), *HitResult.BoneName.ToString());
	if (HitResult.BoneName == "head")
	{
		//UE_LOG(LogTemp, Error, TEXT("Head Shot!!!"));
		return true;
	}
	return false;
}
bool ASuraProjectile::CheckHeadOvelap(const AActor* OverlappedActor, const FHitResult& SweepResult)
{	
	if (!OverlappedActor) return false;

	USkeletalMeshComponent* SkeletalMesh = OverlappedActor->GetComponentByClass<USkeletalMeshComponent>();

	if (SkeletalMesh && SkeletalMesh->DoesSocketExist(FName(TEXT("head"))))
	{
		if (CollisionComp->GetScaledSphereRadius() > FVector::Distance(SweepResult.ImpactPoint, SkeletalMesh->GetBoneLocation(FName(TEXT("head")))))
		{
			//UE_LOG(LogTemp, Error, TEXT("Head Shot!!!"));
			return true;
		}
	}
	return false;
}
#pragma endregion

#pragma region Homing
bool ASuraProjectile::IsTargetValid()
{
	//TODO: Target�� ���� ���θ� �Ǵ��� ����� �ʿ��� -> ���� ���� �����Բ� ��û����� ��
	if (!ProjectileMovement->HomingTargetComponent.IsValid())
	{
		return false;
	}
	return true;
}
bool ASuraProjectile::IsTargetWithInRange()
{
	if (FVector::Distance(GetActorLocation(), RecentTargetLocation) < ExlosionTriggerRadius)
	{
		return true;
	}
	return false;
}
void ASuraProjectile::UpdateTargetInfo()
{
	if (ProjectileMovement->bIsHomingProjectile)
	{
		if (!IsTargetValid() || IsTargetWithInRange()) // TODO: ������ �ӽ÷� ||�� ó����
		{
			//TODO: TargetLocation�� ���� ���ư��� �����ϵ��� �����ϱ�
			//TODO: ������ Target�� ��ġ�� ���� �Ÿ� �̻� ��������� �ڵ� �����ϵ��� �ϴ� �͵� ������ ���� �� ����
			//-> �̴� Target�� ������ο� ��� ���� �����ϴ� ���� ���� ��

			UE_LOG(LogTemp, Error, TEXT("Target is not valid!!!"));

			SpawnExplosionEffect(GetActorLocation());
			ApplyExplosiveDamage(bIsExplosive, GetActorLocation());
			Destroy();
		}
		else
		{
			if (TargetEnemy && IsValid(TargetEnemy))
			{
				RecentTargetLocation = TargetEnemy->GetActorLocation();
			}
		}
	}
}
#pragma endregion

#pragma region Impulse
void ASuraProjectile::AddImpulseToEnemy(AActor* OtherActor, FVector Force)
{
	if (OtherActor != nullptr && IsValid(OtherActor))
	{
		ACharacter* Enemy = Cast<ACharacter>(OtherActor);
		if (Enemy)
		{
			Enemy->LaunchCharacter(Force, false, false);
		}
	}
}
bool ASuraProjectile::CheckRicochetAngle(FVector normal, FVector vel)
{
	return FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(-normal.GetSafeNormal(), vel.GetSafeNormal()))) > MinIncidenceAngle;
}
FVector ASuraProjectile::GetReflectionAngle(FVector normal, FVector input)
{
	FVector norm = normal.GetSafeNormal();
	FVector in = input.GetSafeNormal();

	return in - 2 * (FVector::DotProduct(in, norm) * norm);
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
	UpdateTargetInfo();

	if (bIsHitScan)
	{
		//Projectile Movement Update

	}
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


