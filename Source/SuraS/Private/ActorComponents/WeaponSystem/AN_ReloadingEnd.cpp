// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/WeaponSystem/AN_ReloadingEnd.h"
#include "ActorComponents/WeaponSystem/WeaponInterface.h"

void UAN_ReloadingEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (MeshComp->GetOwner() && MeshComp->GetOwner()->GetClass()->ImplementsInterface(UWeaponInterface::StaticClass()))
	{
		//UE_LOG(LogTemp, Warning, TEXT("Reloading End!!!"));
		Cast<IWeaponInterface>(MeshComp->GetOwner())->ReloadingEnd();
	}
}
