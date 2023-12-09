#pragma once

#include <memory>

#include "../../MessageHandler.h"
#include "server/application/commandHandlers/message/createChannelMessageCommandHandler/CreateChannelMessageCommandHandler.h"
#include "server/application/services/tokenService/TokenService.h"

namespace server::api
{
class SendChannelMessageHandler : public MessageHandler
{
public:
    SendChannelMessageHandler(
        std::shared_ptr<application::TokenService> tokenService,
        std::unique_ptr<application::CreateChannelMessageCommandHandler> createChannelMessageCommandHandler);

    common::messages::Message handleMessage(const common::messages::Message& message) const override;

private:
    std::shared_ptr<application::TokenService> tokenService;
    std::unique_ptr<application::CreateChannelMessageCommandHandler> createChannelMessageCommandHandler;
};
}