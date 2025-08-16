#pragma once

#include "CoreMinimal.h"

namespace EffectDuration
{
    const float Deflect = 2.0f;
    const float Guard = 2.0f;
    const float DefaultCooldown = 5.0f;
};

namespace Animation
{
    const float GuardBlendSpeed = 10.0f;
};

namespace AI
{
    // TODO: Maybe move the keys to sub namespace
    namespace Keys
    {
        const FName ChaseTarget = "ChaseTarget";
        const FName LastKnownLocation = "LastKnownLocation";
        const FName IsPatrolling = "IsPatroling";
        const FName IsChasing = "IsChasing";
        const FName HasInit = "HasInit";
        const FName PatrolTo = "PatrolTo";
        const FName RotateTo = "PatrolRotateTo";
        const FName WaitFor = "WaitTIme";
        const FName HasVisited = "HasInit";
        const FName AnimIdle = "IdleAnimation";
        const FName Finished = "Finished";
    }
};

namespace AssetPath
{
    namespace InputAction
    {
        const auto GuardAction = TEXT("/Game/ThirdPerson/Input/Actions/IA_Guard.IA_Guard");
        const auto LockonAction = TEXT("/Game/ThirdPerson/Input/Actions/IA_Lockon.IA_Lockon");
        const auto LookAction = TEXT("/Game/ThirdPerson/Input/Actions/IA_Look.IA_Look");
        const auto SprintAction = TEXT("/Game/ThirdPerson/Input/Actions/IA_Sprint.IA_Sprint");
    }
    namespace DataTable
    {
        const auto Health = TEXT("/Game/ThirdPerson/DataTable/DT_Health.DT_Health");
        const auto Stamina = TEXT("/Game/ThirdPerson/DataTable/DT_Stamina.DT_Stamina");
    }
    namespace AnimMontage
    {
        const auto FallDownAndRecover = TEXT("/Game/ThirdPerson/Anim/FallDown_Montage.FallDown_Montage");
        const auto StandUp = TEXT("/Game/ThirdPerson/Anim/StandingUp_Montage.StandingUp_Montage");
        const auto DefaultPatrolAnim = TEXT("/Game/ThirdPerson/Anim/StandingUp_Montage.StandingUp_Montage");
    }
};