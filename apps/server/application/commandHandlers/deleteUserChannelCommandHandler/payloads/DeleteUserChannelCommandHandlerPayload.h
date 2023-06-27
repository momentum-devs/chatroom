#pragma once

#include <string>

namespace server::application
{
struct DeleteUserChannelCommandHandlerPayload
{
    const std::string userChannelId;
};
}
