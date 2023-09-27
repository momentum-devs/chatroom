#include "ConversationMapperImpl.h"

namespace server::infrastructure
{
ConversationMapperImpl::ConversationMapperImpl(std::shared_ptr<UserMapper> userMapperInit,
                                               std::shared_ptr<ChannelMapper> channelMapperInit)
    : userMapper{std::move(userMapperInit)}, channelMapper{std::move(channelMapperInit)}
{
}

domain::Conversation ConversationMapperImpl::mapToDomainConversation(const Conversation& conversation) const
{
    const auto id = conversation.getId();
    const auto user = conversation.getUser() ? userMapper->mapToDomainUser(conversation.getUser()) : nullptr;
    const auto recipient =
        conversation.getRecipient() ? userMapper->mapToDomainUser(conversation.getRecipient()) : nullptr;
    const auto channel =
        conversation.getChannel() ? channelMapper->mapToDomainChannel(conversation.getChannel()) : nullptr;
    const auto createdAt = conversation.getCreatedAt();
    const auto updatedAt = conversation.getUpdatedAt();

    return domain::Conversation{id, user, recipient, channel, createdAt, updatedAt};
}
}
