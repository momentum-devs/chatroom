#pragma once

#include <optional>
#include <string>

namespace server::domain
{
struct FindUsersGroupsByGroupIdPayload
{
    std::string groupId;
};
}
