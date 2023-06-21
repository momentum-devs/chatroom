#pragma once

#include "ChannelMapper.h"

namespace server::infrastructure
{
class ChannelMapperImpl : public ChannelMapper
{
public:
    domain::Channel mapToDomainChannel(const Channel&) const override;
};
}
