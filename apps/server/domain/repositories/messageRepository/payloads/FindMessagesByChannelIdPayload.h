#pragma once

#include <optional>
#include <string>

namespace server::domain
{
struct FindMessagesByChannelIdPayload
{
    std::string channelId;
    unsigned offset = 0;
    unsigned limit = 50;
};
}
