#pragma once

#include <memory>

#include "../../MessageHandler.h"
#include "server/application/commandHandlers/message/createMessageReactionCommandHandler/CreateMessageReactionCommandHandler.h"
#include "server/application/services/tokenService/TokenService.h"

namespace server::api
{
class AddReactionMessageHandler : public MessageHandler
{
public:
    AddReactionMessageHandler(
        std::shared_ptr<application::TokenService> tokenService,
        std::unique_ptr<application::CreateMessageReactionCommandHandler> createMessageReactionCommandHandler);

    common::messages::Message handleMessage(const common::messages::Message& message) const override;

private:
    std::shared_ptr<application::TokenService> tokenService;
    std::unique_ptr<application::CreateMessageReactionCommandHandler> createMessageReactionCommandHandler;
};
}