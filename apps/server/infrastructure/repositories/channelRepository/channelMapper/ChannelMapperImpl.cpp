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
    const auto avatarUrl =
        channel->getAvatarUrl().null() ? std::optional<std::string>(std::nullopt) : channel->getAvatarUrl().get();

    return std::make_shared<domain::Channel>(id, name, creator, createdAt, updatedAt, avatarUrl);
}

std::shared_ptr<Channel> ChannelMapperImpl::mapToPersistenceChannel(std::shared_ptr<domain::Channel> channel) const
{
    const auto id = channel->getId();
    const auto name = channel->getName();
    const auto creator = userMapper->mapToPersistenceUser(channel->getCreator());
    const auto createdAt = channel->getCreatedAt();
    const auto updatedAt = channel->getUpdatedAt();
    const auto avatarUrl = channel->getAvatarUrl() ? *channel->getAvatarUrl() : odb::nullable<std::string>();

    return std::make_shared<Channel>(id, name, creator, createdAt, updatedAt, avatarUrl);
}
}
