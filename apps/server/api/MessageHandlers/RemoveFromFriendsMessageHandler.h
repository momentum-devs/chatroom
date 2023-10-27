#pragma once

#include <memory>

#include "server/api/MessageHandler.h"
#include "server/application/commandHandlers/friend/deleteFriendshipCommandHandler/DeleteFriendshipCommandHandler.h"
#include "server/application/services/tokenService/TokenService.h"

namespace server::api
{
class RemoveFromFriendsMessageHandler : public MessageHandler
{
public:
    RemoveFromFriendsMessageHandler(
        std::shared_ptr<server::application::TokenService> tokenService,
        std::unique_ptr<server::application::DeleteFriendshipCommandHandler> deleteFriendshipCommandHandler);
    common::messages::Message handleMessage(const common::messages::Message& message) const override;

private:
    std::shared_ptr<server::application::TokenService> tokenService;
    std::unique_ptr<server::application::DeleteFriendshipCommandHandler> deleteFriendshipCommandHandler;
};
}