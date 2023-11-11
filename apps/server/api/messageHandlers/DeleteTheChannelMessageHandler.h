#pragma once

#include <memory>

#include "MessageHandler.h"
#include "server/application/commandHandlers/channel/deleteChannelCommandHandler/DeleteChannelCommandHandler.h"
#include "server/application/services/tokenService/TokenService.h"

namespace server::api
{
class DeleteTheChannelMessageHandler : public MessageHandler
{
public:
    DeleteTheChannelMessageHandler(
        std::shared_ptr<server::application::TokenService> tokenService,
        std::unique_ptr<server::application::DeleteChannelCommandHandler> deleteChannelCommandHandler);
    common::messages::Message handleMessage(const common::messages::Message& message) const override;

private:
    std::shared_ptr<server::application::TokenService> tokenService;
    std::unique_ptr<server::application::DeleteChannelCommandHandler> deleteChannelCommandHandler;
};
}
