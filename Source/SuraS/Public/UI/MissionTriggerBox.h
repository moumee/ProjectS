// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "MissionTriggerBox.generated.h"

/**
 * 
 */
UCLASS()
class SURAS_API AMissionTriggerBox : public ATriggerBox
{
	GENERATED_BODY()

public:
	AMissionTriggerBox();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger", meta = (ToolTip = "체크 : 타겟 마크 띄울 객체 지정"))
	AActor* MissionTarget;

protected:
	bool bHasBeenTriggered;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger", meta = (ToolTip = "체크 : 한 번만 표시"))
	bool bTriggerOnceOnly;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Trigger", meta = (ToolTip = "체크 : 오버랩시 마크 제거"))
	bool bClearTargetOnTrigger; // 기본값은 false

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
};
