#pragma once

#include <optional>
#include <string>

namespace server::domain
{
struct FindMessagesByGroupIdPayload
{
    std::string groupId;
};
}
