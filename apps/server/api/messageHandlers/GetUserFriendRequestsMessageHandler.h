#pragma once

#include <memory>

#include "MessageHandler.h"
#include "server/application/queryHandlers/friend/findReceivedFriendInvitationsQueryHandler/FindReceivedFriendInvitationsQueryHandler.h"
#include "server/application/services/tokenService/TokenService.h"

namespace server::api
{
class GetUserFriendRequestsMessageHandler : public MessageHandler
{
public:
    GetUserFriendRequestsMessageHandler(std::shared_ptr<application::TokenService> tokenService,
                                        std::unique_ptr<application::FindReceivedFriendInvitationsQueryHandler>
                                            findReceivedFriendInvitationsQueryHandler);
    
    common::messages::Message handleMessage(const common::messages::Message& message) const override;

private:
    std::shared_ptr<application::TokenService> tokenService;
    std::unique_ptr<application::FindReceivedFriendInvitationsQueryHandler> findReceivedFriendInvitationsQueryHandler;
};
}
