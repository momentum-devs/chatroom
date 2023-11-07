#pragma once

#include <memory>

#include "CreateChannelCommandHandler.h"
#include "server/application/commandHandlers/channel/addUserToChannelCommandHandler/AddUserToChannelCommandHandler.h"
#include "server/domain/repositories/channelRepository/ChannelRepository.h"
#include "server/domain/repositories/conversationRepository/ConversationRepository.h"
#include "server/domain/repositories/userRepository/UserRepository.h"

namespace server::application
{
class CreateChannelCommandHandlerImpl : public CreateChannelCommandHandler
{
public:
    CreateChannelCommandHandlerImpl(std::shared_ptr<domain::ChannelRepository>,
                                    std::shared_ptr<AddUserToChannelCommandHandler>,
                                    std::shared_ptr<domain::UserRepository>,
                                    std::shared_ptr<domain::ConversationRepository>);

    CreateChannelCommandHandlerResult execute(const CreateChannelCommandHandlerPayload&) const override;

private:
    std::shared_ptr<domain::ChannelRepository> channelRepository;
    std::shared_ptr<AddUserToChannelCommandHandler> addUserToChannelCommandHandler;
    std::shared_ptr<domain::UserRepository> userRepository;
    std::shared_ptr<domain::ConversationRepository> conversationRepository;
};
}
