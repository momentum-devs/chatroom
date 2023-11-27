#pragma once

#include <optional>
#include <string>

namespace server::domain
{
struct CountMessagesByChannelIdPayload
{
    std::string channelId;
};
}
