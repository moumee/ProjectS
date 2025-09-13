// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Enemies/Animations/SuraBossPartAnimInstance.h"

#include "GameFramework/Character.h"

void USuraBossPartAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (ACharacter* OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner()))
	{
		LeaderMeshComp = OwnerCharacter->GetMesh();
	}
}
