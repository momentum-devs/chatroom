#pragma once

#include <memory>

#include "AcceptChannelInvitationCommandHandler.h"
#include "server/domain/repositories/channelInvitationRepository/ChannelInvitationRepository.h"
#include "server/domain/repositories/channelRepository/ChannelRepository.h"
#include "server/domain/repositories/userChannelRepository/UserChannelRepository.h"
#include "server/domain/repositories/userRepository/UserRepository.h"

namespace server::application
{
class AcceptChannelInvitationCommandHandlerImpl : public AcceptChannelInvitationCommandHandler
{
public:
    AcceptChannelInvitationCommandHandlerImpl(std::shared_ptr<domain::ChannelInvitationRepository>,
                                              std::shared_ptr<domain::UserRepository>,
                                              std::shared_ptr<domain::UserChannelRepository>,
                                              std::shared_ptr<domain::ChannelRepository>);

    void execute(const AcceptChannelInvitationCommandHandlerPayload&) const override;

private:
    void addUserToChannel(const std::string& userId, const std::string& channelId) const;

    std::shared_ptr<domain::ChannelInvitationRepository> channelInvitationRepository;
    std::shared_ptr<domain::UserRepository> userRepository;
    std::shared_ptr<domain::UserChannelRepository> userChannelRepository;
    std::shared_ptr<domain::ChannelRepository> channelRepository;
};
}
