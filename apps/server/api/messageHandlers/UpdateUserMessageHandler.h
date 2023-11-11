#pragma once

#include <memory>

#include "MessageHandler.h"
#include "server/application/commandHandlers/user/updateUserCommandHandler/UpdateUserCommandHandler.h"
#include "server/application/services/tokenService/TokenService.h"

namespace server::api
{
class UpdateUserMessageHandler : public MessageHandler
{
public:
    UpdateUserMessageHandler(std::shared_ptr<server::application::TokenService> tokenService,
                             std::unique_ptr<server::application::UpdateUserCommandHandler> updateUserCommandHandler);
    common::messages::Message handleMessage(const common::messages::Message& message) const override;

private:
    std::shared_ptr<server::application::TokenService> tokenService;
    std::unique_ptr<server::application::UpdateUserCommandHandler> updateUserCommandHandler;
};
}
