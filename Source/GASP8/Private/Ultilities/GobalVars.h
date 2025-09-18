#pragma once

#include "CoreMinimal.h"

namespace EffectDuration
{
    inline constexpr float Deflect = 2.0f;
    inline constexpr float Guard = 2.0f;
    inline constexpr float DefaultCooldown = 5.0f;
};

namespace Animation
{
    inline constexpr float GuardBlendSpeed = 10.0f;
};

namespace AI
{
    namespace Keys
    {
        extern const FName ChaseTarget;
        extern const FName LastKnownLocation;
        extern const FName IsPatrolling;
        extern const FName IsChasing;
    }
};

namespace AssetPath
{
    namespace InputAction
    {
        extern const wchar_t *GuardAction;
        extern const wchar_t *LockonAction;
        extern const wchar_t *LookAction;
        extern const wchar_t *SprintAction;
    }
    namespace DataTable
    {
        extern const wchar_t *Health;
        extern const wchar_t *Stamina;
    }
    namespace AnimMontage
    {
        extern const wchar_t *FallDownAndRecover;
        extern const wchar_t *StandUp;
    }
};