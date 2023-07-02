#pragma once

#include <vector>

#include "../../../../domain/entities/channel/Channel.h"

namespace server::application
{
struct FindChannelsToWhichUserBelongsQueryHandlerResult
{
    std::vector<domain::Channel> channels;
};
}
