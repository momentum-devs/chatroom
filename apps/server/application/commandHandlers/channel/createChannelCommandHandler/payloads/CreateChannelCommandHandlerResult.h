#pragma once

#include "../../../../../domain/entities/channel/Channel.h"

namespace server::application
{
struct CreateChannelCommandHandlerResult
{
    domain::Channel channel;
};
}
