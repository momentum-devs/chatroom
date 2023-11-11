#pragma once

#include <memory>

#include "../../MessageHandler.h"
#include "server/application/commandHandlers/user/verifyUserEmailCommandHandler/VerifyUserEmailCommandHandler.h"
#include "server/application/queryHandlers/user/findUserQueryHandler/FindUserQueryHandler.h"
#include "server/application/services/tokenService/TokenService.h"

namespace server::api
{
class VerifyUserMessageHandler : public MessageHandler
{
public:
    VerifyUserMessageHandler(std::shared_ptr<application::TokenService> tokenService,
                             std::unique_ptr<application::VerifyUserEmailCommandHandler> verifyUserEmailCommandHandler,
                             std::shared_ptr<application::FindUserQueryHandler> findUserQueryHandler);

    common::messages::Message handleMessage(const common::messages::Message& message) const override;

private:
    std::shared_ptr<application::TokenService> tokenService;
    std::unique_ptr<application::VerifyUserEmailCommandHandler> verifyUserEmailCommandHandler;
    std::shared_ptr<application::FindUserQueryHandler> findUserQueryHandler;
};
}
