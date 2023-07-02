#pragma once

#include "../../../../domain/entities/channel/Channel.h"
#include "../../../database/entities/Channel.h"

namespace server::infrastructure
{
class ChannelMapper
{
public:
    virtual ~ChannelMapper() = default;

    virtual std::shared_ptr<domain::Channel> mapToDomainChannel(std::shared_ptr<Channel>) const = 0;
    virtual std::shared_ptr<Channel> mapToPersistenceChannel(std::shared_ptr<domain::Channel>) const = 0;
};
}
