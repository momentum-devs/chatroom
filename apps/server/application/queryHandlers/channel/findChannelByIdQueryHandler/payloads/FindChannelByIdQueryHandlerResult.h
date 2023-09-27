#pragma once

#include <vector>

#include "../../../../../domain/entities/channel/Channel.h"

namespace server::application
{
struct FindChannelByIdQueryHandlerResult
{
    domain::Channel channel;
};
}
