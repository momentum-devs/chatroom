#include "UpdateMessageCommandHandlerImpl.h"

#include "fmt/format.h"
#include "loguru.hpp"
#include "server/application/errors/OperationNotValidError.h"
#include "server/application/errors/ResourceNotFoundError.h"

namespace server::application
{
UpdateMessageCommandHandlerImpl::UpdateMessageCommandHandlerImpl(
    std::shared_ptr<domain::MessageRepository> messageRepositoryInit)
    : messageRepository{std::move(messageRepositoryInit)}
{
}

UpdateMessageCommandHandlerResult
UpdateMessageCommandHandlerImpl::execute(const UpdateMessageCommandHandlerPayload& payload) const
{
    LOG_S(INFO) << fmt::format(R"(Updating message with id "{}" with content "{}"...)", payload.id, payload.content);

    auto existingMessage = messageRepository->findMessageById({payload.id});

    if (!existingMessage)
    {
        throw errors::ResourceNotFoundError{fmt::format("Message with id \"{}\" not found.", payload.id)};
    }

    const auto senderId = existingMessage->get()->getSender()->getId();

    if (payload.requesterUserId != senderId)
    {
        throw errors::OperationNotValidError{fmt::format("User with id {} is not sender of the message with id {}.",
                                                         payload.requesterUserId, payload.id)};
    }

    existingMessage->get()->setContent(payload.content);

    const auto message = messageRepository->updateMessage({**existingMessage});

    LOG_S(INFO) << fmt::format(R"(Message with id "{}" updated with content "{}".)", payload.id, payload.content);

    return {*message};
}
}
