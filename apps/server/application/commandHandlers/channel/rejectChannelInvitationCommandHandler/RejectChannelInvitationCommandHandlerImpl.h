#pragma once

#include <memory>

#include "RejectChannelInvitationCommandHandler.h"
#include "server/domain/repositories/channelInvitationRepository/ChannelInvitationRepository.h"
#include "server/domain/repositories/channelRepository/ChannelRepository.h"
#include "server/domain/repositories/userRepository/UserRepository.h"

namespace server::application
{
class RejectChannelInvitationCommandHandlerImpl : public RejectChannelInvitationCommandHandler
{
public:
    RejectChannelInvitationCommandHandlerImpl(std::shared_ptr<domain::ChannelInvitationRepository>,
                                              std::shared_ptr<domain::UserRepository>);

    void execute(const RejectChannelInvitationCommandHandlerPayload&) const override;

private:
    std::shared_ptr<domain::ChannelInvitationRepository> channelInvitationRepository;
    std::shared_ptr<domain::UserRepository> userRepository;
};
}
