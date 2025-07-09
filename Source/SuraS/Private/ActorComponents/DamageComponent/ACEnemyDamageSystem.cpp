// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/DamageComponent/ACEnemyDamageSystem.h"
#include "Characters/Enemies/SuraCharacterEnemyBase.h"
#include "Characters/Enemies/AI/EnemyBaseAIController.h"

TSet<FName> UACEnemyDamageSystem::HeadBoneNames = {FName(TEXT("head")), FName(TEXT("neck_01"))};
TSet<FName> UACEnemyDamageSystem::LArmBoneNames = {FName(TEXT("upperarm_l")), FName(TEXT("lowerarm_l")), FName(TEXT("hand_l"))};
TSet<FName> UACEnemyDamageSystem::RArmBoneNames = {FName(TEXT("upperarm_r")), FName(TEXT("lowerarm_r")), FName(TEXT("hand_r"))};
TSet<FName> UACEnemyDamageSystem::LLegBoneNames = {FName(TEXT("thigh_l")), FName(TEXT("calf_l")), FName(TEXT("foot_l")), FName(TEXT("ball_l"))};
TSet<FName> UACEnemyDamageSystem::RLegBoneNames = {FName(TEXT("thigh_r")), FName(TEXT("calf_r")), FName(TEXT("foot_r")), FName(TEXT("ball_r"))};

// Sets default values for this component's properties
UACEnemyDamageSystem::UACEnemyDamageSystem()
{
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
}

bool UACEnemyDamageSystem::TakeDamage(const FDamageData& DamageData, const AActor* DamageCauser)
{
	// 1. Get hit location and subtract it from the relevant part of the body
	// 2. Calculate the whole health based on the subtract factor on each body
	
	AActor* OwningEnemyActor = GetOwner();

	/*if (ASuraCharacterEnemyBase* const Enemy = Cast<ASuraCharacterEnemyBase>(OwningEnemyActor))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Enemy Damaged"));
		// Enemy->GetAIController()->AlertNearByEnemies();
	}*/

	if (HeadBoneNames.Contains((DamageData.BoneName)))
	{
		HeadHealth -= DamageData.DamageAmount;
		if (HeadHealth <= 0)
		{
			OwningEnemyActor->GetComponentByClass<USkeletalMeshComponent>()
				->HideBoneByName(FName(TEXT("head")), PBO_Term);
			FVector SpawnLocation = OwningEnemyActor->FindComponentByClass<USkeletalMeshComponent>()
				->GetSocketLocation(FName(TEXT("head")));
			if (RArm != nullptr)
			{
				GetWorld()->SpawnActor<AActor>(Head, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
			}
		}
	}
	if (LArmBoneNames.Contains((DamageData.BoneName)))
	{
		LArmHealth -= DamageData.DamageAmount;
		if (LArmHealth <= 0)
		{
			OwningEnemyActor->GetComponentByClass<USkeletalMeshComponent>()
				->HideBoneByName(FName(TEXT("upperarm_l")), PBO_Term);
			FVector SpawnLocation = OwningEnemyActor->FindComponentByClass<USkeletalMeshComponent>()
				->GetSocketLocation(FName(TEXT("upperarm_l")));
			if (RArm != nullptr)
			{
				GetWorld()->SpawnActor<AActor>(LArm, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
			}
		}
	}
	if (RArmBoneNames.Contains((DamageData.BoneName)))
	{
		RArmHealth -= DamageData.DamageAmount;
		if (RArmHealth <= 0)
		{
			OwningEnemyActor->GetComponentByClass<USkeletalMeshComponent>()
				->HideBoneByName(FName(TEXT("upperarm_r")), PBO_Term);
			FVector SpawnLocation = OwningEnemyActor->FindComponentByClass<USkeletalMeshComponent>()
				->GetSocketLocation(FName(TEXT("upperarm_r")));
			if (RArm != nullptr)
			{
				GetWorld()->SpawnActor<AActor>(RArm, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
			}
			
		}
	}
	if (LLegBoneNames.Contains((DamageData.BoneName)))
	{
		LLegHealth -= DamageData.DamageAmount;
		if (LLegHealth <= 0) OwningEnemyActor->GetComponentByClass<USkeletalMeshComponent>()
		->HideBoneByName(FName(TEXT("thigh_l")), PBO_Term);
	}
	if (RLegBoneNames.Contains((DamageData.BoneName)))
	{
		RLegHealth -= DamageData.DamageAmount;
		if (RLegHealth <= 0) OwningEnemyActor->GetComponentByClass<USkeletalMeshComponent>()
		->HideBoneByName(FName(TEXT("thigh_r")), PBO_Term);
	}
	
	UE_LOG(LogTemp, Error, TEXT("bone: %s"), *DamageData.BoneName.ToString());
	
	return Super::TakeDamage(DamageData, DamageCauser);
}