#pragma once

#include <optional>
#include <string>

namespace server::domain
{
struct FindUserGroupPayload
{
    std::string userId;
    std::string groupId;
};
}
