#pragma once

#include <memory>

#include "server/api/MessageHandler.h"
#include "server/application/commandHandlers/registerUserCommandHandler/RegisterUserCommandHandler.h"

namespace server::api
{
class RegisterMessageHandler : public MessageHandler
{
public:
    RegisterMessageHandler(std::unique_ptr<server::application::RegisterUserCommandHandler> registerUserCommandHandler);
    common::messages::Message handleMessage(const common::messages::Message& message) const override;

private:
    std::unique_ptr<server::application::RegisterUserCommandHandler> registerUserCommandHandler;
};
}