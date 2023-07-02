#include "ChannelMapperImpl.h"

namespace server::infrastructure
{
std::shared_ptr<domain::Channel> ChannelMapperImpl::mapToDomainChannel(std::shared_ptr<Channel> channel) const
{
    const auto id = channel->getId();
    const auto name = channel->getName();
    const auto creatorId = channel->getCreatorId();
    const auto createdAt = channel->getCreatedAt();
    const auto updatedAt = channel->getUpdatedAt();

    return std::make_shared<domain::Channel>(id, name, creatorId, createdAt, updatedAt);
}

std::shared_ptr<Channel> ChannelMapperImpl::mapToPersistenceChannel(std::shared_ptr<domain::Channel> channel) const
{
    const auto id = channel->getId();
    const auto name = channel->getName();
    const auto creatorId = channel->getCreatorId();
    const auto createdAt = channel->getCreatedAt();
    const auto updatedAt = channel->getUpdatedAt();

    return std::make_shared<Channel>(id, name, creatorId, createdAt, updatedAt);
}
}
