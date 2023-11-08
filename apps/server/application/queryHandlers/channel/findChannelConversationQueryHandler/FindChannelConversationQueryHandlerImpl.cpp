#include "FindChannelConversationQueryHandlerImpl.h"

#include <format>

#include "server/application/errors/ResourceNotFoundError.h"

namespace server::application
{
FindChannelConversationQueryHandlerImpl::FindChannelConversationQueryHandlerImpl(
    std::shared_ptr<domain::ConversationRepository> conversationRepositoryInit)
    : conversationRepository{std::move(conversationRepositoryInit)}
{
}

FindChannelConversationQueryHandlerResult
FindChannelConversationQueryHandlerImpl::execute(const FindChannelConversationQueryHandlerPayload& payload) const
{
    const auto conversation = conversationRepository->findConversationByChannelId({payload.channelId});

    if (!conversation)
    {
        throw errors::ResourceNotFoundError{
            std::format("Conversation with channel id \"{}\" not found.", payload.channelId)};
    }

    return {*conversation};
}
}
