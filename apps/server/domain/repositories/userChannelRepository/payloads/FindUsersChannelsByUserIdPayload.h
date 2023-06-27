#pragma once

#include <optional>
#include <string>

namespace server::domain
{
struct FindUsersChannelsByUserIdPayload
{
    std::string userId;
};
}
