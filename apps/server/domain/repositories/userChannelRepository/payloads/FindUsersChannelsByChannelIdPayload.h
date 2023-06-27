#pragma once

#include <optional>
#include <string>

namespace server::domain
{
struct FindUsersChannelsByChannelIdPayload
{
    std::string channelId;
};
}
