#include "MessageMapperImpl.h"

#include <utility>

namespace server::infrastructure
{
MessageMapperImpl::MessageMapperImpl(std::shared_ptr<UserMapper> userMapperInit,
                                     std::shared_ptr<ChannelMapper> channelMapperInit,
                                     std::shared_ptr<GroupMapper> groupMapperInit)
    : userMapper{std::move(userMapperInit)},
      channelMapper{std::move(channelMapperInit)},
      groupMapper{std::move(groupMapperInit)}
{
}

std::shared_ptr<domain::Message> MessageMapperImpl::mapToDomainMessage(std::shared_ptr<Message> message) const
{
    const auto id = message->getId();
    const auto content = message->getContent();
    const auto sender = userMapper->mapToDomainUser(message->getSender());
    const auto channel = message->getChannel() ? channelMapper->mapToDomainChannel(message->getChannel()) : nullptr;
    const auto group = message->getGroup() ? groupMapper->mapToDomainGroup(message->getGroup()) : nullptr;
    const auto createdAt = message->getCreatedAt();
    const auto updatedAt = message->getUpdatedAt();

    return std::make_shared<domain::Message>(id, content, sender, channel, group, createdAt, updatedAt);
}

std::shared_ptr<Message>
MessageMapperImpl::mapToPersistenceMessage(const std::shared_ptr<domain::Message> message) const
{
    const auto id = message->getId();
    const auto content = message->getContent();
    const auto sender = userMapper->mapToPersistenceUser(message->getSender());
    const auto channel =
        message->getChannel() ? channelMapper->mapToPersistenceChannel(message->getChannel()) : nullptr;
    const auto group = message->getGroup() ? groupMapper->mapToPersistenceGroup(message->getGroup()) : nullptr;
    const auto createdAt = message->getCreatedAt();
    const auto updatedAt = message->getUpdatedAt();

    return std::make_shared<Message>(id, content, sender, channel, group, createdAt, updatedAt);
}

}
