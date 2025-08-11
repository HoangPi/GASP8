#pragma once

#include "CoreMinimal.h"

namespace EffectDuration
{
    const float Deflect = 2.0f;
    const float Guard = 2.0f;
    const float DefaultCooldown = 5.0f;
};

namespace AI
{
    namespace Keys
    {
        const FName ChaseTarget = "ChaseTarget";
        const FName LastKnownLocation = "LastKnownLocation";
        const FName IsPatrolling = "IsPatroling";
        const FName IsChasing = "IsChasing";
    }
};