#pragma once

#include <string>

namespace server::application
{
struct DeleteChannelCommandHandlerPayload
{
    const std::string channelId;
};
}
