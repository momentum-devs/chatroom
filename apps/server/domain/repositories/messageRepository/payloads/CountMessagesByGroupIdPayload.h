#pragma once

#include <optional>
#include <string>

namespace server::domain
{
struct CountMessagesByGroupIdPayload
{
    std::string groupId;
};
}
