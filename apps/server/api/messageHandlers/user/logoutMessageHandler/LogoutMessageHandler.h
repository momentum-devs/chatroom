#pragma once

#include <memory>

#include "../../MessageHandler.h"
#include "server/application/commandHandlers/user/logoutUserCommandHandler/LogoutUserCommandHandler.h"
#include "server/application/services/tokenService/TokenService.h"

namespace server::api
{
class LogoutMessageHandler : public MessageHandler
{
public:
    LogoutMessageHandler(std::shared_ptr<application::TokenService> tokenService,
                         std::unique_ptr<application::LogoutUserCommandHandler> logoutUserCommandHandler);

    common::messages::Message handleMessage(const common::messages::Message& message) const override;

private:
    std::shared_ptr<application::TokenService> tokenService;
    std::unique_ptr<application::LogoutUserCommandHandler> logoutUserCommandHandler;
};
}
