#include "GobalVars.h"

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

namespace AssetPath
{
    namespace InputAction
    {
        const wchar_t *GuardAction = TEXT("/Game/ThirdPerson/Input/Actions/IA_Guard.IA_Guard");
        const wchar_t *LockonAction = TEXT("/Game/ThirdPerson/Input/Actions/IA_Lockon.IA_Lockon");
        const wchar_t *LookAction = TEXT("/Game/ThirdPerson/Input/Actions/IA_Look.IA_Look");
        const wchar_t *SprintAction = TEXT("/Game/ThirdPerson/Input/Actions/IA_Sprint.IA_Sprint");
    }
    namespace DataTable
    {
        const wchar_t *Health = TEXT("/Game/ThirdPerson/DataTable/DT_Health.DT_Health");
        const wchar_t *Stamina = TEXT("/Game/ThirdPerson/DataTable/DT_Stamina.DT_Stamina");
    }
    namespace AnimMontage
    {
        const wchar_t *FallDownAndRecover = TEXT("/Game/ThirdPerson/Anim/FallDown_Montage.FallDown_Montage");
        const wchar_t *StandUp = TEXT("/Game/ThirdPerson/Anim/StandingUp_Montage.StandingUp_Montage");
    }
};