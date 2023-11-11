#pragma once

#include <memory>

#include "CreateChannelMessageCommandHandler.h"
#include "server/domain/repositories/channelRepository/ChannelRepository.h"
#include "server/domain/repositories/messageRepository/MessageRepository.h"
#include "server/domain/repositories/userChannelRepository/UserChannelRepository.h"
#include "server/domain/repositories/userRepository/UserRepository.h"

namespace server::application
{
class CreateChannelMessageCommandHandlerImpl : public CreateChannelMessageCommandHandler
{
public:
    CreateChannelMessageCommandHandlerImpl(std::shared_ptr<domain::ChannelRepository>,
                                           std::shared_ptr<domain::UserRepository>,
                                           std::shared_ptr<domain::UserChannelRepository>,
                                           std::shared_ptr<domain::MessageRepository>);

    CreateChannelMessageCommandHandlerResult execute(const CreateChannelMessageCommandHandlerPayload&) const override;

private:
    std::shared_ptr<domain::ChannelRepository> channelRepository;
    std::shared_ptr<domain::UserRepository> userRepository;
    std::shared_ptr<domain::UserChannelRepository> userChannelRepository;
    std::shared_ptr<domain::MessageRepository> messageRepository;
};
}
