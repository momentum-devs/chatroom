#pragma once

#include <vector>

#include "../../../../../domain/entities/friendship/Friendship.h"

namespace server::application
{
struct FindUserFriendshipsQueryHandlerResult
{
    std::vector<domain::Friendship> friendships;
};
}
