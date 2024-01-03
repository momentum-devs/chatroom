#include "DeleteMessageCommandHandlerImpl.h"

#include "fmt/format.h"
#include "server/application/errors/OperationNotValidError.h"
#include "server/application/errors/ResourceNotFoundError.h"

namespace server::application
{
DeleteMessageCommandHandlerImpl::DeleteMessageCommandHandlerImpl(
    std::shared_ptr<domain::MessageRepository> messageRepositoryInit)
    : messageRepository{std::move(messageRepositoryInit)}
{
}

void DeleteMessageCommandHandlerImpl::execute(const DeleteMessageCommandHandlerPayload& payload)
{
    const auto existingMessage = messageRepository->findMessageById({payload.messageId});

    if (!existingMessage)
    {
        throw errors::ResourceNotFoundError{fmt::format("Message with id {} not found.", payload.messageId)};
    }

    const auto senderId = existingMessage->get()->getSender()->getId();

    if (payload.requesterUserId != senderId)
    {
        throw errors::OperationNotValidError{fmt::format("User with id {} is not sender of the message with id {}.",
                                                         payload.requesterUserId, payload.messageId)};
    }

    messageRepository->deleteMessage({**existingMessage});
}
}
