#pragma once

#include <memory>

#include "server/api/MessageHandler.h"
#include "server/application/queryHandlers/channel/findReceivedChannelInvitationsQueryHandler/FindReceivedChannelInvitationsQueryHandler.h"
#include "server/application/services/tokenService/TokenService.h"

namespace server::api
{
class GetUserChannelInvitationsMessageHandler : public MessageHandler
{
public:
    GetUserChannelInvitationsMessageHandler(
        std::shared_ptr<server::application::TokenService> tokenService,
        std::unique_ptr<server::application::FindReceivedChannelInvitationsQueryHandler>
            findReceivedChannelInvitationsQueryHandler);
    common::messages::Message handleMessage(const common::messages::Message& message) const override;

private:
    std::shared_ptr<server::application::TokenService> tokenService;
    std::unique_ptr<server::application::FindReceivedChannelInvitationsQueryHandler>
        findReceivedChannelInvitationsQueryHandler;
};
}