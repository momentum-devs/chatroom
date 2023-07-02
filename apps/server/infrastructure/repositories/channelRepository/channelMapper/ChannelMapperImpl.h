#pragma once

#include "ChannelMapper.h"

namespace server::infrastructure
{
class ChannelMapperImpl : public ChannelMapper
{
public:
    std::shared_ptr<domain::Channel> mapToDomainChannel(std::shared_ptr<Channel>) const override;
    std::shared_ptr<Channel> mapToPersistenceChannel(std::shared_ptr<domain::Channel>) const override;
};
}
