#pragma once

#include <string>

#include "../../../entities/channel/Channel.h"

namespace server::domain
{
struct DeleteChannelPayload
{
    Channel channel;
};
}
