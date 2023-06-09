#pragma once

#include <memory>

#include "server/api/MessageHandler.h"
#include "server/application/commandHandlers/deleteUserCommandHandler/DeleteUserCommandHandler.h"
#include "server/application/services/tokenService/TokenService.h"

namespace server::api
{
class DeleteUserMessageHandler : public MessageHandler
{
public:
    DeleteUserMessageHandler(std::shared_ptr<server::application::TokenService> tokenService,
                             std::unique_ptr<server::application::DeleteUserCommandHandler> deleteUserCommandHandler);
    common::messages::Message handleMessage(const common::messages::Message& message) const override;

private:
    std::shared_ptr<server::application::TokenService> tokenService;
    std::unique_ptr<server::application::DeleteUserCommandHandler> deleteUserCommandHandler;
};
}