// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/DamageComponent/ACEnemyDamageSystem.h"
#include "Characters/Enemies/SuraCharacterEnemyBase.h"
#include "Characters/Enemies/AI/EnemyBaseAIController.h"

TSet<FName> UACEnemyDamageSystem::HeadBoneNames = {FName(TEXT("head"))};
TSet<FName> UACEnemyDamageSystem::LArmBoneNames = {FName(TEXT("upperarm_l")), FName(TEXT("lowerarm_l")), FName(TEXT("hand_l"))};
TSet<FName> UACEnemyDamageSystem::RArmBoneNames = {FName(TEXT("upperarm_r")), FName(TEXT("lowerarm_r")), FName(TEXT("hand_r"))};
TSet<FName> UACEnemyDamageSystem::LLegBoneNames = {FName(TEXT("thigh_l")), FName(TEXT("calf_l")), FName(TEXT("foot_l")), FName(TEXT("ball_l"))};
TSet<FName> UACEnemyDamageSystem::RLegBoneNames = {FName(TEXT("thigh_r")), FName(TEXT("calf_r")), FName(TEXT("foot_r")), FName(TEXT("ball_r"))};
//TArray<float> UACEnemyDamageSystem::ImpulsePower = 

// Sets default values for this component's properties
UACEnemyDamageSystem::UACEnemyDamageSystem()
{
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	//if (const auto EnemyAttributesData = EnemyAttributesDT.DataTable->FindRow<FEnemyAttributesData>(EnemyType, ""))
	
}



bool UACEnemyDamageSystem::TakeDamage(const FDamageData& DamageData, AActor* DamageCauser)
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
			PartBroken(OwningEnemyActor, DamageData, MaxHealth ,"head", Head);
		}
	}
	if (LArmBoneNames.Contains((DamageData.BoneName)))
	{
		LArmHealth -= DamageData.DamageAmount;
		if (LArmHealth <= 0)
		{
			PartBroken(OwningEnemyActor, DamageData, MaxLArmHealth, "upperarm_l", LArm);
			Cast<ASuraCharacterEnemyBase>(OwningEnemyActor)->SetCrippled();
		}
	}
	if (RArmBoneNames.Contains((DamageData.BoneName)))
	{
		RArmHealth -= DamageData.DamageAmount;
		if (RArmHealth <= 0)
		{
			PartBroken(OwningEnemyActor, DamageData, MaxRArmHealth, "upperarm_r", RArm);
			Cast<ASuraCharacterEnemyBase>(OwningEnemyActor)->SetCrippled();
		}
	}
	if (LLegBoneNames.Contains((DamageData.BoneName)))
	{
		LLegHealth -= DamageData.DamageAmount;
		if (LLegHealth <= 0)
		{
			PartBroken(OwningEnemyActor, DamageData, MaxLLegHealth, "thigh_l", LLeg);
			Cast<ASuraCharacterEnemyBase>(OwningEnemyActor)->SetCrippled();
			
		}
		
	}
	if (RLegBoneNames.Contains((DamageData.BoneName)))
	{
		RLegHealth -= DamageData.DamageAmount;
		if (RLegHealth <= 0)
		{
			PartBroken(OwningEnemyActor, DamageData, MaxRLegHealth, "thigh_r", RLeg);
			Cast<ASuraCharacterEnemyBase>(OwningEnemyActor)->SetCrippled();
		}
		
	}
	
	return Super::TakeDamage(DamageData, DamageCauser);
}

void UACEnemyDamageSystem::SetImpulsePower(float weak, float normal, float hard)
{
	ImpulsePower.Empty();
	ImpulsePower.Add(weak);
	ImpulsePower.Add(normal);
	ImpulsePower.Add(hard);
}

void UACEnemyDamageSystem::SetPartsHealth(float head, float body, float rArm, float lArm, float lLeg, float rLeg)
{
	MaxHeadHealth = head;	HeadHealth = head;
	MaxBodyHealth = body;	BodyHealth = body;
	MaxRArmHealth = rArm;	RArmHealth = rArm;
	MaxLArmHealth = lArm;	LArmHealth = lArm;
	MaxRLegHealth = rLeg;	RLegHealth = rLeg;
	MaxLLegHealth = lLeg;	LLegHealth = lLeg;
}

float UACEnemyDamageSystem::CalculateImpulsePower(float Damage, float PartMaxHealth)
{
	float DamageRatio = Damage/PartMaxHealth*3;
	if (DamageRatio < 1) return ImpulsePower[0];
	if (DamageRatio < 2) return ImpulsePower[1];
	return ImpulsePower[2];
}

void UACEnemyDamageSystem::PartBroken(AActor* OwningEnemyActor, const FDamageData& DamageData, float PartMaxHealth, const FName PartsParent, TSubclassOf<AActor> SeparatedPart)
{
	OwningEnemyActor->GetComponentByClass<USkeletalMeshComponent>()
				->HideBoneByName(PartsParent, PBO_Term);
	FVector SpawnLocation = OwningEnemyActor->FindComponentByClass<USkeletalMeshComponent>()
		->GetSocketLocation(PartsParent);

	if (SeparatedPart != nullptr)
	{
		AActor* bodyPart;
		bodyPart = GetWorld()->SpawnActor<AActor>(SeparatedPart, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
			
		UPrimitiveComponent* Enemy = Cast<UPrimitiveComponent>(bodyPart->GetRootComponent());
		UE_LOG(LogTemp, Error, TEXT("vector: %s"), *(DamageData.ImpulseDirection).ToString());
		if (Enemy)
		{
			bodyPart->SetActorRotation(OwningEnemyActor->GetComponentByClass<USkeletalMeshComponent>()->GetSocketRotation(PartsParent));
			Enemy->AddImpulse(DamageData.ImpulseDirection * -CalculateImpulsePower(DamageData.DamageAmount, PartMaxHealth) * 3, PartsParent, true );
		}
	}
}
