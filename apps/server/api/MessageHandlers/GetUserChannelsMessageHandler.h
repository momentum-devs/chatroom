#pragma once

#include <memory>

#include "server/api/MessageHandler.h"
#include "server/application/queryHandlers/findChannelsToWhichUserBelongsQueryHandler/FindChannelsToWhichUserBelongsQueryHandler.h"
#include "server/application/services/tokenService/TokenService.h"

namespace server::api
{
class GetUserChannelsMessageHandler : public MessageHandler
{
public:
    GetUserChannelsMessageHandler(std::shared_ptr<server::application::TokenService> tokenService,
                                  std::unique_ptr<server::application::FindChannelsToWhichUserBelongsQueryHandler>
                                      findChannelsToWhichUserBelongsQueryHandler);
    common::messages::Message handleMessage(const common::messages::Message& message) const override;

private:
    std::shared_ptr<server::application::TokenService> tokenService;
    std::unique_ptr<server::application::FindChannelsToWhichUserBelongsQueryHandler>
        findChannelsToWhichUserBelongsQueryHandler;
};
}