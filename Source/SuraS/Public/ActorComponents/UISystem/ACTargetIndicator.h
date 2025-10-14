// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ACTargetIndicator.generated.h"


class UTargetIndicatorWidget;
class UWidgetComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SURAS_API UACTargetIndicator : public UActorComponent
{
	GENERATED_BODY()

public:
	UACTargetIndicator();
	
	void ShowIndicator() const;
	
	void HideIndicator() const;

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category = "Indicator")
	TSubclassOf<UTargetIndicatorWidget> IndicatorWidgetClass;

private:
	// C++ 코드 내에서 생성하고 관리하므로 UPROPERTY()만 붙여 GC(가비지 컬렉션)에 의해 제거되지 않도록 함
	UPROPERTY()
	UWidgetComponent* IndicatorWidgetComponent;
};
