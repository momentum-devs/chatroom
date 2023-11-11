#pragma once

#include <memory>

#include "server/api/MessageHandler.h"
#include "server/application/commandHandlers/channel/createChannelInvitationCommandHandler/CreateChannelInvitationCommandHandler.h"
#include "server/application/queryHandlers/user/findUserByEmailQueryHandler/FindUserByEmailQueryHandler.h"
#include "server/application/services/tokenService/TokenService.h"

namespace server::api
{
class SendChannelInvitationMessageHandler : public MessageHandler
{
public:
    SendChannelInvitationMessageHandler(
        std::shared_ptr<server::application::TokenService> tokenService,
        std::shared_ptr<server::application::FindUserByEmailQueryHandler> findUserByEmailQueryHandler,
        std::unique_ptr<server::application::CreateChannelInvitationCommandHandler>
            createChannelInvitationCommandHandler);
    common::messages::Message handleMessage(const common::messages::Message& message) const override;

private:
    std::shared_ptr<server::application::TokenService> tokenService;
    std::shared_ptr<server::application::FindUserByEmailQueryHandler> findUserByEmailQueryHandler;
    std::unique_ptr<server::application::CreateChannelInvitationCommandHandler> createChannelInvitationCommandHandler;
};
}
