#include "ChannelMapperImpl.h"

namespace server::infrastructure
{
ChannelMapperImpl::ChannelMapperImpl(std::shared_ptr<UserMapper> userMapperInit) : userMapper{std::move(userMapperInit)}
{
}

std::shared_ptr<domain::Channel> ChannelMapperImpl::mapToDomainChannel(std::shared_ptr<Channel> channel) const
{
    const auto id = channel->getId();
    const auto name = channel->getName();
    const auto creator = userMapper->mapToDomainUser(channel->getCreator());
    const auto createdAt = channel->getCreatedAt();
    const auto updatedAt = channel->getUpdatedAt();

    return std::make_shared<domain::Channel>(id, name, creator, createdAt, updatedAt);
}

std::shared_ptr<Channel> ChannelMapperImpl::mapToPersistenceChannel(std::shared_ptr<domain::Channel> channel) const
{
    const auto id = channel->getId();
    const auto name = channel->getName();
    const auto creator = userMapper->mapToPersistenceUser(channel->getCreator());
    const auto createdAt = channel->getCreatedAt();
    const auto updatedAt = channel->getUpdatedAt();

    return std::make_shared<Channel>(id, name, creator, createdAt, updatedAt);
}
}
