#pragma once

#include <memory>

#include "../../MessageHandler.h"
#include "server/application/queryHandlers/message/findGroupMessagesQueryHandler/FindGroupMessagesQueryHandler.h"
#include "server/application/services/tokenService/TokenService.h"

namespace server::api
{
class GetMessagesFromGroupMessageHandler : public MessageHandler
{
public:
    GetMessagesFromGroupMessageHandler(
        std::shared_ptr<application::TokenService> tokenService,
        std::unique_ptr<application::FindGroupMessagesQueryHandler> findGroupMessagesQueryHandler);

    common::messages::Message handleMessage(const common::messages::Message& message) const override;

private:
    std::shared_ptr<application::TokenService> tokenService;
    std::unique_ptr<application::FindGroupMessagesQueryHandler> findGroupMessagesQueryHandler;
};
}