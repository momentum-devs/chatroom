#pragma once

#include <memory>

#include "MessageHandler.h"
#include "server/application/commandHandlers/channel/leaveChannelCommandHandler/LeaveChannelCommandHandler.h"
#include "server/application/services/tokenService/TokenService.h"

namespace server::api
{
class LeaveChannelMessageHandler : public MessageHandler
{
public:
    LeaveChannelMessageHandler(std::shared_ptr<application::TokenService> tokenService,
                               std::unique_ptr<application::LeaveChannelCommandHandler> leaveChannelCommandHandler);

    common::messages::Message handleMessage(const common::messages::Message& message) const override;

private:
    std::shared_ptr<application::TokenService> tokenService;
    std::unique_ptr<application::LeaveChannelCommandHandler> leaveChannelCommandHandler;
};
}
