#pragma once

#include <memory>

#include "../../MessageHandler.h"
#include "server/application/commandHandlers/friend/createFriendInvitationCommandHandler/CreateFriendInvitationCommandHandler.h"
#include "server/application/queryHandlers/user/findUserByEmailQueryHandler/FindUserByEmailQueryHandler.h"
#include "server/application/services/tokenService/TokenService.h"

namespace server::api
{
class FriendInvitationMessageHandler : public MessageHandler
{
public:
    FriendInvitationMessageHandler(
        std::shared_ptr<application::TokenService> tokenService,
        std::shared_ptr<application::FindUserByEmailQueryHandler> findUserByEmailQueryHandler,
        std::unique_ptr<application::CreateFriendInvitationCommandHandler> createFriendInvitationCommandHandler);

    common::messages::Message handleMessage(const common::messages::Message& message) const override;

private:
    std::shared_ptr<application::TokenService> tokenService;
    std::shared_ptr<application::FindUserByEmailQueryHandler> findUserByEmailQueryHandler;
    std::unique_ptr<application::CreateFriendInvitationCommandHandler> createFriendInvitationCommandHandler;
};
}
