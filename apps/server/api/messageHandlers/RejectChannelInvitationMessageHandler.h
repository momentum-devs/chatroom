#pragma once

#include <memory>

#include "MessageHandler.h"
#include "server/application/commandHandlers/channel/rejectChannelInvitationCommandHandler/RejectChannelInvitationCommandHandler.h"
#include "server/application/services/tokenService/TokenService.h"

namespace server::api
{
class RejectChannelInvitationMessageHandler : public MessageHandler
{
public:
    RejectChannelInvitationMessageHandler(std::shared_ptr<server::application::TokenService> tokenService,
                                          std::unique_ptr<server::application::RejectChannelInvitationCommandHandler>
                                              rejectChannelInvitationCommandHandler);
    common::messages::Message handleMessage(const common::messages::Message& message) const override;

private:
    std::shared_ptr<server::application::TokenService> tokenService;
    std::unique_ptr<server::application::RejectChannelInvitationCommandHandler> rejectChannelInvitationCommandHandler;
};
}
