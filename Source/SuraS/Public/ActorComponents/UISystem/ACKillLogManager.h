// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ACKillLogManager.generated.h"

class UKillLogWidget;
class UACUIMangerComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SURAS_API UACKillLogManager : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY()
	UKillLogWidget* KillLogWidget;

	UPROPERTY()
	UACUIMangerComponent* UIManager;

public:	
	// Sets default values for this component's properties
	UACKillLogManager();

	//void AddKillLog(const FString& Killer, const FString& Victim);
	void SetKillLogWidget(UKillLogWidget* InWidget);
	void SetUIManager(UACUIMangerComponent* InManager);
	void AddKillLog(const FString& Killer, const FString& Victim);

	UKillLogWidget* GetKillLogWidget()
	{
		return KillLogWidget;
	}

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
