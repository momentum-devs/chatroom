#pragma once

#include <memory>

#include "MessageHandler.h"
#include "server/application/commandHandlers/user/registerUserCommandHandler/RegisterUserCommandHandler.h"
#include "server/application/commandHandlers/user/sendRegistrationVerificationEmailCommandHandler/SendRegistrationVerificationEmailCommandHandler.h"

namespace server::api
{
class RegisterMessageHandler : public MessageHandler
{
public:
    RegisterMessageHandler(std::unique_ptr<server::application::RegisterUserCommandHandler> registerUserCommandHandler,
                           std::shared_ptr<server::application::SendRegistrationVerificationEmailCommandHandler>
                               sendRegistrationVerificationEmailCommandHandler);
    common::messages::Message handleMessage(const common::messages::Message& message) const override;

private:
    std::unique_ptr<server::application::RegisterUserCommandHandler> registerUserCommandHandler;
    std::shared_ptr<server::application::SendRegistrationVerificationEmailCommandHandler>
        sendRegistrationVerificationEmailCommandHandler;
};
}
