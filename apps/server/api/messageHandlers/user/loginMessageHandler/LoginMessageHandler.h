#pragma once

#include <memory>

#include "../../MessageHandler.h"
#include "server/application/commandHandlers/user/loginUserCommandHandler/LoginUserCommandHandler.h"
#include "server/application/services/tokenService/TokenService.h"

namespace server::api
{
class LoginMessageHandler : public MessageHandler
{
public:
    explicit LoginMessageHandler(std::unique_ptr<application::LoginUserCommandHandler> loginUserCommandHandler);

    common::messages::Message handleMessage(const common::messages::Message& message) const override;

private:
    std::unique_ptr<application::LoginUserCommandHandler> loginUserCommandHandler;
};
}
