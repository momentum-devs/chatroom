#include "DeleteMessageReactionCommandHandlerImpl.h"

#include "fmt/format.h"

#include "loguru.hpp"
#include "server/application/errors/OperationNotValidError.h"
#include "server/application/errors/ResourceNotFoundError.h"

namespace server::application
{
DeleteMessageReactionCommandHandlerImpl::DeleteMessageReactionCommandHandlerImpl(
    std::shared_ptr<domain::UserRepository> userRepositoryInit,
    std::shared_ptr<domain::MessageRepository> messageRepositoryInit,
    std::shared_ptr<domain::ReactionRepository> reactionRepositoryInit)
    : userRepository{std::move(userRepositoryInit)},
      messageRepository{std::move(messageRepositoryInit)},
      reactionRepository{std::move(reactionRepositoryInit)}
{
}

void DeleteMessageReactionCommandHandlerImpl::execute(const DeleteMessageReactionCommandHandlerPayload& payload) const
{
    LOG_S(INFO) << fmt::format(R"(Deleting message reaction... {{reactionId: "{}", messageId: "{}", userId: "{}"}})",
                               payload.reactionId, payload.messageId, payload.userId);

    const auto user = userRepository->findUserById({payload.userId});

    if (!user)
    {
        throw errors::ResourceNotFoundError{fmt::format("User with id {} not found.", payload.userId)};
    }

    const auto message = messageRepository->findMessageById({payload.messageId});

    if (!message)
    {
        throw errors::ResourceNotFoundError{fmt::format("Message with id {} not found.", payload.messageId)};
    }

    const auto reaction = reactionRepository->findReactionById({payload.reactionId});

    if (!reaction)
    {
        throw errors::ResourceNotFoundError{fmt::format("Reaction with id {} not found.", payload.reactionId)};
    }

    if (reaction->getUser()->getId() != user->get()->getId())
    {
        throw errors::OperationNotValidError{fmt::format(
            "User with id {} is not the creator of the reaction with id {}.", user->get()->getId(), reaction->getId())};
    }

    if (reaction->getMessage()->getId() != message->get()->getId())
    {
        throw errors::OperationNotValidError{fmt::format("Reaction with id {} is not associated with message id {}.",
                                                         reaction->getId(), message->get()->getId())};
    }
    
    reactionRepository->deleteReaction({*reaction});

    LOG_S(INFO) << fmt::format(R"(Message reaction deleted. {{reactionId: "{}"}})", reaction->getId());
}

}
