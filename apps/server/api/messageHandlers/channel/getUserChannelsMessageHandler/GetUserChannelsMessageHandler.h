#pragma once

#include <memory>

#include "../../MessageHandler.h"
#include "server/application/queryHandlers/channel/findChannelsToWhichUserBelongsQueryHandler/FindChannelsToWhichUserBelongsQueryHandler.h"
#include "server/application/services/tokenService/TokenService.h"

namespace server::api
{
class GetUserChannelsMessageHandler : public MessageHandler
{
public:
    GetUserChannelsMessageHandler(std::shared_ptr<application::TokenService> tokenService,
                                  std::unique_ptr<application::FindChannelsToWhichUserBelongsQueryHandler>
                                      findChannelsToWhichUserBelongsQueryHandler);

    common::messages::Message handleMessage(const common::messages::Message& message) const override;

private:
    std::shared_ptr<application::TokenService> tokenService;
    std::unique_ptr<application::FindChannelsToWhichUserBelongsQueryHandler> findChannelsToWhichUserBelongsQueryHandler;
};
}
