#pragma once

#include <memory>

#include "../../MessageHandler.h"
#include "server/application/queryHandlers/message/findChannelMessagesQueryHandler/FindChannelMessagesQueryHandler.h"
#include "server/application/services/tokenService/TokenService.h"

namespace server::api
{
class GetMessagesFromChannelMessageHandler : public MessageHandler
{
public:
    GetMessagesFromChannelMessageHandler(
        std::shared_ptr<application::TokenService> tokenService,
        std::unique_ptr<application::FindChannelMessagesQueryHandler> findChannelMessagesQueryHandler);

    common::messages::Message handleMessage(const common::messages::Message& message) const override;

private:
    std::shared_ptr<application::TokenService> tokenService;
    std::unique_ptr<application::FindChannelMessagesQueryHandler> findChannelMessagesQueryHandler;
};
}