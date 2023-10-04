#pragma once

#include <memory>

#include "server/api/MessageHandler.h"
#include "server/application/commandHandlers/friend/createFriendInvitationCommandHandler/CreateFriendInvitationCommandHandler.h"
#include "server/application/queryHandlers/user/findUserByEmailQueryHandler/FindUserByEmailQueryHandler.h"
#include "server/application/services/tokenService/TokenService.h"

namespace server::api
{
class FriendRequestMessageHandler : public MessageHandler
{
public:
    FriendRequestMessageHandler(
        std::shared_ptr<server::application::TokenService> tokenService,
        std::shared_ptr<server::application::FindUserByEmailQueryHandler> findUserByEmailQueryHandler,
        std::unique_ptr<server::application::CreateFriendInvitationCommandHandler>
            createFriendInvitationCommandHandler);
    common::messages::Message handleMessage(const common::messages::Message& message) const override;

private:
    std::shared_ptr<server::application::TokenService> tokenService;
    std::shared_ptr<server::application::FindUserByEmailQueryHandler> findUserByEmailQueryHandler;
    std::unique_ptr<server::application::CreateFriendInvitationCommandHandler> createFriendInvitationCommandHandler;
};
}
