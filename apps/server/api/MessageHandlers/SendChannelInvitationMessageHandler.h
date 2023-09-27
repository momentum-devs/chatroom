#pragma once

#include <memory>

#include "server/api/MessageHandler.h"
#include "server/application/commandHandlers/channel/addUserToChannelCommandHandler/AddUserToChannelCommandHandler.h"
#include "server/application/queryHandlers/user/findUserQueryHandler/FindUserQueryHandler.h"
#include "server/application/services/tokenService/TokenService.h"

namespace server::api
{
class SendChannelInvitationMessageHandler : public MessageHandler
{
public:
    SendChannelInvitationMessageHandler(
        std::shared_ptr<server::application::TokenService> tokenService,
        std::shared_ptr<server::application::FindUserQueryHandler> findUserQueryHandler,
        std::shared_ptr<server::application::AddUserToChannelCommandHandler> addUserToChannelCommandHandler);
    common::messages::Message handleMessage(const common::messages::Message& message) const override;

private:
    std::shared_ptr<server::application::TokenService> tokenService;
    std::shared_ptr<server::application::FindUserQueryHandler> findUserQueryHandler;
    std::shared_ptr<server::application::AddUserToChannelCommandHandler> addUserToChannelCommandHandler;
};
}
