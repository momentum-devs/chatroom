#pragma once

#include <memory>

#include "MessageHandler.h"
#include "server/application/commandHandlers/friend/rejectFriendInvitationCommandHandler/RejectFriendInvitationCommandHandler.h"
#include "server/application/services/tokenService/TokenService.h"

namespace server::api
{
class RejectFriendRequestMessageHandler : public MessageHandler
{
public:
    RejectFriendRequestMessageHandler(std::shared_ptr<application::TokenService> tokenService,
                                      std::unique_ptr<application::RejectFriendInvitationCommandHandler>
                                          rejectFriendInvitationCommandHandler);

    common::messages::Message handleMessage(const common::messages::Message& message) const override;

private:
    std::shared_ptr<application::TokenService> tokenService;
    std::unique_ptr<application::RejectFriendInvitationCommandHandler> rejectFriendInvitationCommandHandler;
};
}
