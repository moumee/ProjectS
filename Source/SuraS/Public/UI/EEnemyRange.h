#pragma once

#include "CoreMinimal.h"
#include "EEnemyRange.generated.h"

UENUM(BlueprintType)
enum class EEnemyRange : uint8
{
	ER_None,
	ER_Melee,
	ER_Ranged
};