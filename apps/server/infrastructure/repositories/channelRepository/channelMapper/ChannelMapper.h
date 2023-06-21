#pragma once

#include "../../../../domain/entities/channel/Channel.h"
#include "../../../database/tables/channelTable/Channel.h"

namespace server::infrastructure
{
class ChannelMapper
{
public:
    virtual ~ChannelMapper() = default;

    virtual domain::Channel mapToDomainChannel(const Channel&) const = 0;
};
}
