#include "FindChannelMessagesQueryHandlerImpl.h"

#include "User.h"

namespace server::application
{
FindChannelMessagesQueryHandlerImpl::FindChannelMessagesQueryHandlerImpl(
    std::shared_ptr<domain::MessageRepository> messageRepositoryInit)
    : messageRepository{std::move(messageRepositoryInit)}
{
}

FindChannelMessagesQueryHandlerResult
FindChannelMessagesQueryHandlerImpl::execute(const FindChannelMessagesQueryHandlerPayload& payload) const
{
    const auto channelMessages =
        messageRepository->findMessagesByChannelId({payload.channelId, payload.offset, payload.limit});

    std::vector<domain::Message> result;

    result.reserve(channelMessages.size());

    for (const auto& channelMessage : channelMessages)
    {
        result.push_back(*channelMessage);
    }

    const auto messageCount = messageRepository->countMessagesByChannelId({payload.channelId});

    return {result, messageCount};
}
}
