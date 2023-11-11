#pragma once

#include <memory>

#include "../MessageHandler.h"
#include "server/application/commandHandlers/channel/createChannelCommandHandler/CreateChannelCommandHandler.h"
#include "server/application/services/tokenService/TokenService.h"

namespace server::api
{
class CreateChannelMessageHandler : public MessageHandler
{
public:
    CreateChannelMessageHandler(std::shared_ptr<application::TokenService> tokenService,
                                std::unique_ptr<application::CreateChannelCommandHandler> createChannelCommandHandler);

    common::messages::Message handleMessage(const common::messages::Message& message) const override;

private:
    std::shared_ptr<application::TokenService> tokenService;
    std::unique_ptr<application::CreateChannelCommandHandler> createChannelCommandHandler;
};
}
