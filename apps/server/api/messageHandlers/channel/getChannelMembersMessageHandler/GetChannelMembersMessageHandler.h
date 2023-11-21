#pragma once

#include <memory>

#include "../../MessageHandler.h"
#include "server/application/queryHandlers/channel/findUsersBelongingToChannelQueryHandler/FindUsersBelongingToChannelQueryHandler.h"
#include "server/application/services/tokenService/TokenService.h"

namespace server::api
{
class GetChannelMembersMessageHandler : public MessageHandler
{
public:
    GetChannelMembersMessageHandler(
        std::shared_ptr<application::TokenService> tokenService,
        std::unique_ptr<application::FindUsersBelongingToChannelQueryHandler> findUsersBelongingToChannelQueryHandler);

    common::messages::Message handleMessage(const common::messages::Message& message) const override;

private:
    std::shared_ptr<application::TokenService> tokenService;
    std::unique_ptr<application::FindUsersBelongingToChannelQueryHandler> findUsersBelongingToChannelQueryHandler;
};
}
