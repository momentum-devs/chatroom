#pragma once

#include <optional>
#include <string>

namespace server::domain
{
struct FindConversationByChannelIdPayload
{
    std::string channelId;
};
}
