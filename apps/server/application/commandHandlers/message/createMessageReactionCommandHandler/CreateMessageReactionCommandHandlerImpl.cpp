#include "CreateMessageReactionCommandHandlerImpl.h"

#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <format>

#include "loguru.hpp"
#include "server/application/errors/OperationNotValidError.h"
#include "server/application/errors/ResourceNotFoundError.h"

namespace server::application
{
CreateMessageReactionCommandHandlerImpl::CreateMessageReactionCommandHandlerImpl(
    std::shared_ptr<domain::UserRepository> userRepositoryInit,
    std::shared_ptr<domain::MessageRepository> messageRepositoryInit,
    std::shared_ptr<domain::ReactionRepository> reactionRepositoryInit)
    : userRepository{std::move(userRepositoryInit)},
      messageRepository{std::move(messageRepositoryInit)},
      reactionRepository{std::move(reactionRepositoryInit)}
{
}

CreateMessageReactionCommandHandlerResult
CreateMessageReactionCommandHandlerImpl::execute(const CreateMessageReactionCommandHandlerPayload& payload) const
{
    LOG_S(INFO) << std::format(R"(Creating message reaction... {{reactionName: "{}", messageId: "{}", userId: "{}"}})",
                               payload.reactionName, payload.messageId, payload.userId);

    const auto user = userRepository->findUserById({payload.userId});

    if (!user)
    {
        throw errors::ResourceNotFoundError{std::format("User with id {} not found.", payload.userId)};
    }

    const auto message = messageRepository->findMessageById({payload.messageId});

    if (!message)
    {
        throw errors::ResourceNotFoundError{std::format("Message with id {} not found.", payload.messageId)};
    }

    std::stringstream uuid;
    uuid << boost::uuids::random_generator()();

    const auto reactionId = uuid.str();

    const auto reaction = reactionRepository->createReaction({reactionId, payload.reactionName, *user, *message});

    LOG_S(INFO) << std::format(R"(Message reaction created. {{reactionId: "{}"}})", reactionId);

    return {reaction};
}

}
