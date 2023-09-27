#pragma once

#include <memory>

#include "server/api/MessageHandler.h"
#include "server/application/commandHandlers/channel/removeUserFromChannelCommandHandler/RemoveUserFromChannelCommandHandler.h"
#include "server/application/services/tokenService/TokenService.h"

namespace server::api
{
class LeftTheChannelMessageHandler : public MessageHandler
{
public:
    LeftTheChannelMessageHandler(
        std::shared_ptr<server::application::TokenService> tokenService,
        std::shared_ptr<server::application::RemoveUserFromChannelCommandHandler> removeUserFromChannelCommandHandler);
    common::messages::Message handleMessage(const common::messages::Message& message) const override;

private:
    std::shared_ptr<server::application::TokenService> tokenService;
    std::shared_ptr<server::application::RemoveUserFromChannelCommandHandler> removeUserFromChannelCommandHandler;
};
}
