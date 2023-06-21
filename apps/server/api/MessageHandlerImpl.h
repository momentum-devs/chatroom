#pragma once

#include <memory>
#include <optional>

#include "MessageHandler.h"
#include "server/application/commandHandlers/loginUserCommandHandler/LoginUserCommandHandler.h"
#include "server/application/commandHandlers/registerUserCommandHandler/RegisterUserCommandHandler.h"

namespace server::api
{
class MessageHandlerImpl : public MessageHandler
{
public:
    MessageHandlerImpl(std::unique_ptr<server::application::RegisterUserCommandHandler> registerUserCommandHandler,
                       std::unique_ptr<server::application::LoginUserCommandHandler> loginUserCommandHandler);

    common::messages::Message handleMessage(const common::messages::Message& message) override;

private:
    common::messages::Message handleRegisterMessage(const common::bytes::Bytes& payload);
    common::messages::Message handleLoginMessage(const common::bytes::Bytes& payload);

    std::unique_ptr<server::application::RegisterUserCommandHandler> registerUserCommandHandler;
    std::unique_ptr<server::application::LoginUserCommandHandler> loginUserCommandHandler;
};
}
