#pragma once

#include "../../../../../domain/entities/channel/Channel.h"

namespace server::application
{
struct UpdateChannelCommandHandlerResult
{
    domain::Channel channel;
};
}
