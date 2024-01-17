#pragma once

#include <memory>

#include "../../MessageHandler.h"
#include "server/application/commandHandlers/message/createGroupMessageCommandHandler/CreateGroupMessageCommandHandler.h"
#include "server/application/services/tokenService/TokenService.h"

namespace server::api
{
class SendMessageToGroupMessageHandler : public MessageHandler
{
public:
    SendMessageToGroupMessageHandler(
        std::shared_ptr<application::TokenService> tokenService,
        std::unique_ptr<application::CreateGroupMessageCommandHandler> createGroupMessageCommandHandler);

    common::messages::Message handleMessage(const common::messages::Message& message) const override;

private:
    std::shared_ptr<application::TokenService> tokenService;
    std::unique_ptr<application::CreateGroupMessageCommandHandler> createGroupMessageCommandHandler;
};
}