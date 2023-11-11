#pragma once

#include <memory>

#include "../MessageHandler.h"
#include "server/application/commandHandlers/channel/acceptChannelInvitationCommandHandler/AcceptChannelInvitationCommandHandler.h"
#include "server/application/services/tokenService/TokenService.h"

namespace server::api
{
class AcceptChannelInvitationMessageHandler : public MessageHandler
{
public:
    AcceptChannelInvitationMessageHandler(
        std::shared_ptr<application::TokenService> tokenService,
        std::unique_ptr<application::AcceptChannelInvitationCommandHandler> acceptChannelInvitationCommandHandler);

    common::messages::Message handleMessage(const common::messages::Message& message) const override;

private:
    std::shared_ptr<application::TokenService> tokenService;
    std::unique_ptr<application::AcceptChannelInvitationCommandHandler> acceptChannelInvitationCommandHandler;
};
}
