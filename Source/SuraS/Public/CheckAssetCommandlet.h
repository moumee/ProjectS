// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Commandlets/Commandlet.h"
#include "CheckAssetCommandlet.generated.h"

/**
 * 
 */
UCLASS()
class SURAS_API UCheckAssetCommandlet : public UCommandlet
{
	GENERATED_BODY()

public:
	UCheckAssetCommandlet();
	virtual int32 Main(const FString& Params) override;
};
