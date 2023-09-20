#pragma once

#include <string>

#include "../../../entities/friendship/Friendship.h"

namespace server::domain
{
struct DeleteFriendshipPayload
{
    Friendship friendship;
};
}
