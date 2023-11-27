#include "FindGroupMessagesQueryHandlerImpl.h"

namespace server::application
{
FindGroupMessagesQueryHandlerImpl::FindGroupMessagesQueryHandlerImpl(
    std::shared_ptr<domain::MessageRepository> messageRepositoryInit)
    : messageRepository{std::move(messageRepositoryInit)}
{
}

FindGroupMessagesQueryHandlerResult
FindGroupMessagesQueryHandlerImpl::execute(const FindGroupMessagesQueryHandlerPayload& payload) const
{
    const auto groupMessages =
        messageRepository->findMessagesByGroupId({payload.groupId, payload.offset, payload.limit});

    std::vector<domain::Message> result;

    result.reserve(groupMessages.size());

    for (const auto& groupMessage : groupMessages)
    {
        result.push_back(*groupMessage);
    }

    const auto messageCount = messageRepository->countMessagesByGroupId({payload.groupId});

    return {result, messageCount};
}
}
