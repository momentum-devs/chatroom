#pragma once

namespace server::infrastructure
{
class ChannelMapper
{
public:
    virtual ~ChannelMapper() = default;

    virtual domain::Channel mapToDomainChannel(const Channel&) const = 0;
};
}
