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

	// 특정 클래스의 객체 가져오기
	UObjectPoolBase* GetPool(AActor* ObjectClass, UObject* WorldContext);

	// 객체 풀에 반환하기
	void ReturnToPool(AActor* Object);


	
private:
	UPROPERTY()
	TMap<AActor*, UObjectPoolBase*> ObjectPool_List;
};
