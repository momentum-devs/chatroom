#pragma once

#include <optional>
#include <string>

namespace server::domain
{
struct FindFriendshipsByUserIdPayload
{
    std::string userId;
};
}
