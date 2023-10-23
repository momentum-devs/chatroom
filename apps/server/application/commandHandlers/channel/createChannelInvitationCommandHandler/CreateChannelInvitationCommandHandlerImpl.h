#pragma once

#include <memory>

#include "CreateChannelInvitationCommandHandler.h"
#include "server/domain/repositories/channelInvitationRepository/ChannelInvitationRepository.h"
#include "server/domain/repositories/channelRepository/ChannelRepository.h"
#include "server/domain/repositories/userChannelRepository/UserChannelRepository.h"
#include "server/domain/repositories/userRepository/UserRepository.h"

namespace server::application
{
class CreateChannelInvitationCommandHandlerImpl : public CreateChannelInvitationCommandHandler
{
public:
    CreateChannelInvitationCommandHandlerImpl(std::shared_ptr<domain::ChannelInvitationRepository>,
                                              std::shared_ptr<domain::UserRepository>,
                                              std::shared_ptr<domain::ChannelRepository>,
                                              std::shared_ptr<domain::UserChannelRepository>);

    void execute(const CreateChannelInvitationCommandHandlerPayload&) const override;

private:
    std::shared_ptr<domain::ChannelInvitationRepository> channelInvitationRepository;
    std::shared_ptr<domain::UserRepository> userRepository;
    std::shared_ptr<domain::ChannelRepository> channelRepository;
    std::shared_ptr<domain::UserChannelRepository> userChannelRepository;
};
}
