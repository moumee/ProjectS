// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ANS_BossAttackArea.generated.h"


class ASuraCharacterBossProto;
enum class EDamageType : uint8;
/**
 * 
 */
UCLASS()
class SURAS_API UANS_BossAttackArea : public UAnimNotifyState
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ASuraCharacterBossProto> BossRef;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDamageType DamageType;

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
};
