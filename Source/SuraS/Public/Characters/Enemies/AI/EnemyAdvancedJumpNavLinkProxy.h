// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Navigation/NavLinkProxy.h"
#include "EnemyAdvancedJumpNavLinkProxy.generated.h"

/**
 * 
 */
UCLASS()
class SURAS_API AEnemyAdvancedJumpNavLinkProxy : public ANavLinkProxy
{
	GENERATED_BODY()
public:
	AEnemyAdvancedJumpNavLinkProxy();

	// 에디터에서 핸들 위치를 편집할 수 있도록 노출되는 경로 포인트 배열입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Navigation", Meta = (MakeEditWidget = true))
	TArray<FVector> PathPoints;

protected:
	// 에디터에서 값이 변경될 때 또는 레벨에 처음 배치될 때 호출됩니다.
	virtual void OnConstruction(const FTransform& Transform) override;

#if WITH_EDITOR
	// 에디터에서 프로퍼티가 수정될 때 호출됩니다.
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

private:
	// 기본 NavLinkProxy의 시작점/끝점을 우리 경로의 첫/마지막 지점과 동기화하는 함수
	void UpdateAndSynchronizeLinks();
};
