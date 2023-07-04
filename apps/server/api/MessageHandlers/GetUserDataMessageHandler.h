#pragma once

#include <memory>

#include "server/api/MessageHandler.h"
#include "server/application/queryHandlers/findUserQueryHandler/FindUserQueryHandler.h"
#include "server/application/services/tokenService/TokenService.h"

namespace server::api
{
class GetUserDataMessageHandler : public MessageHandler
{
public:
    GetUserDataMessageHandler(std::shared_ptr<server::application::TokenService> tokenService,
                              std::unique_ptr<server::application::FindUserQueryHandler> findUserQueryHandler);
    common::messages::Message handleMessage(const common::messages::Message& message) const override;

private:
    std::shared_ptr<server::application::TokenService> tokenService;
    std::unique_ptr<server::application::FindUserQueryHandler> findUserQueryHandler;
};
}