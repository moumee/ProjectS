// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ACUIMangerComponent.h"
#include "Components/ActorComponent.h"
#include "ACSkillManager.generated.h"


class URocketLauncherSkillWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SURAS_API UACSkillManager : public UActorComponent
{
	GENERATED_BODY()

	URocketLauncherSkillWidget* RocketLauncherSkillWidget;
	UACUIMangerComponent* UIManager;

public:	
	// Sets default values for this component's properties
	UACSkillManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetRocketLauncherSkillWidget(URocketLauncherSkillWidget* IW);
	void SetUIManager(UACUIMangerComponent* InManager);

	// 무기 델리게이트에 함수를 바인딩하는 핵심 함수
	void BindWeaponSkillDelegate();
};
