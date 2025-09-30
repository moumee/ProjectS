// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ObjectPoolBase.h"
#include "HAL/CriticalSection.h"
#include "ObjectPoolManager.generated.h"



/**
 * 
 */
UCLASS()
class SURAS_API UObjectPoolManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UObjectPoolManager();

	// Ư�� Ŭ������ ��ü ��������
	UObjectPoolBase* GetPool(TSubclassOf<AActor> ObjectClass, UObject* WorldContext);

	// ��ü Ǯ�� ��ȯ�ϱ�
	void ReturnToPool(AActor* Object);


	
private:
	UPROPERTY()
	TMap<TSubclassOf<AActor>, UObjectPoolBase*> ObjectPool_List;
};
