#pragma once

#include <string>

namespace server::application
{
struct RemoveUserFromChannelCommandHandlerPayload
{
    const std::string userId;
    const std::string channelId;
};
}
