#pragma once

#include <memory>

#include "AcceptChannelInvitationCommandHandler.h"
#include "server/application/commandHandlers/channel/addUserToChannelCommandHandler/AddUserToChannelCommandHandler.h"
#include "server/domain/repositories/channelInvitationRepository/ChannelInvitationRepository.h"
#include "server/domain/repositories/channelRepository/ChannelRepository.h"
#include "server/domain/repositories/userRepository/UserRepository.h"

namespace server::application
{
class AcceptChannelInvitationCommandHandlerImpl : public AcceptChannelInvitationCommandHandler
{
public:
    AcceptChannelInvitationCommandHandlerImpl(std::shared_ptr<domain::ChannelInvitationRepository>,
                                              std::shared_ptr<domain::UserRepository>,
                                              std::shared_ptr<application::AddUserToChannelCommandHandler>);

    void execute(const AcceptChannelInvitationCommandHandlerPayload&) const override;

private:
    std::shared_ptr<domain::ChannelInvitationRepository> channelInvitationRepository;
    std::shared_ptr<domain::UserRepository> userRepository;
    std::shared_ptr<application::AddUserToChannelCommandHandler> addUserToChannelCommandHandler;
};
}
