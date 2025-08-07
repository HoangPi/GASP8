#pragma once

#include "CoreMinimal.h"

/**
 * Custom enum to demonstrate how to expose enums to Blueprints.
 */
UENUM(BlueprintType)
enum class ETeamEnum : uint8
{
    Player UMETA(DisplayName = "Player"),
    Enemy UMETA(DisplayName = "Enemy"),
};