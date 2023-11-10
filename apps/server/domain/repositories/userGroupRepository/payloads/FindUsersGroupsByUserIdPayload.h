#pragma once

#include <optional>
#include <string>

namespace server::domain
{
struct FindUsersGroupsByUserIdPayload
{
    std::string userId;
};
}
