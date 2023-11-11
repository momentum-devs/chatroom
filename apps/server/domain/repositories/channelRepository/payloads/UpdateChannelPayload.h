#pragma once

#include <string>

#include "../../../entities/channel/Channel.h"

namespace server::domain
{
struct UpdateChannelPayload
{
    Channel channel;
};
}
