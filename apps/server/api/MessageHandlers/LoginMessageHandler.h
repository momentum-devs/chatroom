#pragma once

#include <memory>

#include "server/api/MessageHandler.h"
#include "server/application/commandHandlers/loginUserCommandHandler/LoginUserCommandHandler.h"
#include "server/application/services/tokenService/TokenService.h"

namespace server::api
{
class LoginMessageHandler : public MessageHandler
{
public:
    LoginMessageHandler(std::shared_ptr<server::application::TokenService> tokenService,
                        std::unique_ptr<server::application::LoginUserCommandHandler> loginUserCommandHandler);
    common::messages::Message handleMessage(const common::messages::Message& message) const override;

private:
    std::unique_ptr<server::application::LoginUserCommandHandler> loginUserCommandHandler;
    std::shared_ptr<server::application::TokenService> tokenService;
};
}