#include "ChannelMapperImpl.h"

namespace server::infrastructure
{
domain::Channel ChannelMapperImpl::mapToDomainChannel(const Channel& channel) const
{
    const auto id = channel.getId();
    const auto name = channel.getName();
    const auto creatorId = channel.getCreatorId();
    const auto createdAt = channel.getCreatedAt();
    const auto updatedAt = channel.getUpdatedAt();

    return domain::Channel{id, name, creatorId, createdAt, updatedAt};
}
}
