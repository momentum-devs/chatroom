#pragma once

#include <memory>

#include "MessageHandler.h"
#include "server/application/commandHandlers/friend/acceptFriendInvitationCommandHandler/AcceptFriendInvitationCommandHandler.h"
#include "server/application/services/tokenService/TokenService.h"

namespace server::api
{
class AcceptFriendRequestMessageHandler : public MessageHandler
{
public:
    AcceptFriendRequestMessageHandler(std::shared_ptr<application::TokenService> tokenService,
                                      std::unique_ptr<application::AcceptFriendInvitationCommandHandler>
                                          acceptFriendInvitationCommandHandler);

    common::messages::Message handleMessage(const common::messages::Message& message) const override;

private:
    std::shared_ptr<application::TokenService> tokenService;
    std::unique_ptr<application::AcceptFriendInvitationCommandHandler> acceptFriendInvitationCommandHandler;
};
}
