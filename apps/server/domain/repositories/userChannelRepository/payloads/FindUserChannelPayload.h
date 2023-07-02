#pragma once

#include <optional>
#include <string>

namespace server::domain
{
struct FindUserChannelPayload
{
    std::string userId;
    std::string channelId;
};
}
