#pragma once

#include <optional>
#include <string>

namespace server::domain
{
struct FindFriendshipByUserIdsPayload
{
    std::string userId;
    std::string userFriendId;
};
}
