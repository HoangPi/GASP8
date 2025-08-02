#include "MyTags/PlayerState/PlayerState.h"

namespace Tags
{
    namespace PlayerState
    {
        UE_DEFINE_GAMEPLAY_TAG(combat, "player_state.combat")
        UE_DEFINE_GAMEPLAY_TAG(manual_guard, "player_state.manual_guard")
        UE_DEFINE_GAMEPLAY_TAG(on_air, "player_state.on_air")
        UE_DEFINE_GAMEPLAY_TAG(should_move, "player_state.should_move")
        UE_DEFINE_GAMEPLAY_TAG(disabled, "player_state.disabled")
    }
}
