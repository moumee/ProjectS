// Fill out your copyright notice in the Description page of Project Settings.


#include "Utilities/EQS/PlayerLocationContext.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "Engine/World.h"

void UPlayerLocationContext::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	if (AActor* Player = GetWorld()->GetFirstPlayerController()->GetPawn())
	{
	}
}
