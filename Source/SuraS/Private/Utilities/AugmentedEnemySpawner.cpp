// Fill out your copyright notice in the Description page of Project Settings.


#include "Utilities/AugmentedEnemySpawner.h"

#include "NavigationSystem.h"
#include "ActorComponents/DamageComponent/ACEnemyDamageSystem.h"
#include "Characters/Enemies/SuraCharacterEnemyBase.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Characters/Enemies/AI/EnemyBaseAIController.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"

void AAugmentedEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	if (EnemiesToWatch.Num() > 0)
	{
		NumOfEnemiesToWatch = EnemiesToWatch.Num();
		
		for (auto Enemy : EnemiesToWatch)
		{
			if (Enemy && Enemy->GetDamageSystemComp())
				Enemy->GetDamageSystemComp()->OnDeath.AddUObject(this, &AAugmentedEnemySpawner::OnDeathTriggered);
		}
	}
}

void AAugmentedEnemySpawner::OnDeathTriggered()
{
	NumOfEnemiesToWatch--;

	ASuraPawnPlayer* Player = Cast<ASuraPawnPlayer>(GetWorld()->GetFirstPlayerController()->GetPawn());

	if (NumOfEnemiesToWatch <= 0)
	{
		EnemiesToWatch.Empty();

		for (auto [EnemyTypeToSpawn, EnemyBT, NumOfEnemiesToSpawn] : EnemiesToSpawn)
		{
			for (int32 i = 0; i < NumOfEnemiesToSpawn; i++)
			{
				FVector SpawnLocation;
				bool bLocationFound = FindValidSpawnLocation(EnemyTypeToSpawn, SpawnLocation, MaxSpawnAttempts);

				FRotator LookAtRotation =  UKismetMathLibrary::FindLookAtRotation(SpawnLocation, Player->GetActorLocation());
				FRotator SpawnRotation = FRotator(0, LookAtRotation.Yaw, 0);

				if (bLocationFound)
				{
					APawn* SpawnedPawn = UAIBlueprintHelperLibrary::SpawnAIFromClass(GetWorld(), EnemyTypeToSpawn, EnemyBT, SpawnLocation, SpawnRotation, true);

					if (ASuraCharacterEnemyBase* SpawnedEnemy = Cast<ASuraCharacterEnemyBase>(SpawnedPawn))
					{
						if (SpawnedEnemy->GetAIController())
							SpawnedEnemy->GetAIController()->SetStateToChaseOrPursue(Player);
					}
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Could not find a valid spawn location after %d attempts."), MaxSpawnAttempts);
				}
			}
		}
	}
}

bool AAugmentedEnemySpawner::FindValidSpawnLocation(TSubclassOf<ASuraCharacterEnemyBase> EnemyClass, FVector& OutLocation, int32 MaxAttempts)
{
	UNavigationSystemV1* Navigation = UNavigationSystemV1::GetCurrent(GetWorld());
    if (!Navigation)
    {
        OutLocation = GetActorLocation();
        return false;
    }

    // use the half-height of the spawned enemy's capsule component to prevent spawning inside the ground
    float HalfHeight = 0.0f;
    if (EnemyClass)
    {
        if (AActor* DefaultActor = EnemyClass->GetDefaultObject<AActor>())
        {
            if (UCapsuleComponent* CapsuleComp = DefaultActor->FindComponentByClass<UCapsuleComponent>())
            {
                HalfHeight = CapsuleComp->GetScaledCapsuleHalfHeight();
            }
            else // if no capsule component, but I doubt
            {
                HalfHeight = 90.0f; // Typical enemy character half-height
            }
        }
    }

    for (int32 i = 0; i < MaxAttempts; ++i)
    {
	    FNavLocation RandomLocationOnNavMesh;
    	if (Navigation->GetRandomPointInNavigableRadius(GetActorLocation(), SpawnRadius, RandomLocationOnNavMesh))
    	{
    		FVector TestLocation = RandomLocationOnNavMesh.Location;
    		TestLocation.Z += HalfHeight; 

    		// Sphere trace to check for overlaps
    		FCollisionShape CollisionSphere = FCollisionShape::MakeSphere(MinSpawnDistanceBtwEnemies);
    		
    		FCollisionQueryParams QueryParams;
    		QueryParams.AddIgnoredActor(this); // Ignore the spawner itself
    		
    		bool bOverlap = GetWorld()->OverlapAnyTestByChannel(
				TestLocation, 
				FQuat::Identity, 
				ECollisionChannel::ECC_GameTraceChannel6,
				CollisionSphere, 
				QueryParams
			);

    		if (!bOverlap)
    		{
    			// a downward line trace to snap to the ground.
    			FHitResult HitResult;
    			FVector TraceStart = TestLocation + FVector(0,0,HalfHeight + 10.0f); // Start slightly above predicted spot
    			FVector TraceEnd = TestLocation - FVector(0,0,HalfHeight + 10.0f); // Go slightly below predicted spot

    			FCollisionQueryParams TraceQueryParams;
    			TraceQueryParams.AddIgnoredActor(this); // Ignore the spawner itself
                
    			if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, TraceQueryParams)) // Use Visibility or WorldStatic
    			{
    				OutLocation = HitResult.Location + FVector(0,0,HalfHeight); // Place half-height above the hit point
    				// UE_LOG(LogTemp, TEXT("Found valid spawn location after trace: %s"), *OutLocation.ToString());
    				return true;
    			}
    			else
    			{
    				// If line trace fails
    				OutLocation = TestLocation;
    				UE_LOG(LogTemp, Warning, TEXT("Found valid spawn location from navmesh (no trace hit): %s"), *OutLocation.ToString());
    				return true;
    			}
    		}
    		else
    		{
    			UE_LOG(LogTemp, Warning, TEXT("Overlapped when spawning somehow"));
    		}
    	}
    }

	OutLocation = GetActorLocation(); // if no valid location is found
	return false;
}

/*FVector AAugmentedEnemySpawner::GetRandomLocation()
{
	if (UNavigationSystemV1* const Navigation = UNavigationSystemV1::GetCurrent(GetWorld()))
	{
		FNavLocation RandomLocation;

		if (Navigation->GetRandomPointInNavigableRadius(GetActorLocation(), 100.f, RandomLocation))
			return RandomLocation;
	}

	return GetActorLocation();
}*/
