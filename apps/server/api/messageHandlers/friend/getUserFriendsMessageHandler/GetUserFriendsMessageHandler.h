#pragma once

#include <memory>

#include "../../MessageHandler.h"
#include "server/application/queryHandlers/friend/findUserFriendshipsQueryHandler/FindUserFriendshipsQueryHandler.h"
#include "server/application/services/tokenService/TokenService.h"

namespace server::api
{
class GetUserFriendsMessageHandler : public MessageHandler
{
public:
    GetUserFriendsMessageHandler(
        std::shared_ptr<application::TokenService> tokenService,
        std::unique_ptr<application::FindUserFriendshipsQueryHandler> findUserFriendshipsQueryHandler);

    common::messages::Message handleMessage(const common::messages::Message& message) const override;

private:
    std::shared_ptr<application::TokenService> tokenService;
    std::unique_ptr<application::FindUserFriendshipsQueryHandler> findUserFriendshipsQueryHandler;
};
}
