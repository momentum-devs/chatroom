#pragma once

#include <memory>

#include "CreateMessageReactionCommandHandler.h"
#include "server/domain/repositories/channelRepository/ChannelRepository.h"
#include "server/domain/repositories/messageRepository/MessageRepository.h"
#include "server/domain/repositories/reactionRepository/ReactionRepository.h"
#include "server/domain/repositories/userChannelRepository/UserChannelRepository.h"
#include "server/domain/repositories/userRepository/UserRepository.h"

namespace server::application
{
class CreateMessageReactionCommandHandlerImpl : public CreateMessageReactionCommandHandler
{
public:
    CreateMessageReactionCommandHandlerImpl(std::shared_ptr<domain::UserRepository>,
                                            std::shared_ptr<domain::MessageRepository>,
                                            std::shared_ptr<domain::ReactionRepository>);

    CreateMessageReactionCommandHandlerResult execute(const CreateMessageReactionCommandHandlerPayload&) const override;

private:
    std::shared_ptr<domain::UserRepository> userRepository;
    std::shared_ptr<domain::MessageRepository> messageRepository;
    std::shared_ptr<domain::ReactionRepository> reactionRepository;
};
}
