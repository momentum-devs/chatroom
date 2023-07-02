#pragma once

#include <memory>
#include <optional>

#include "MessageHandler.h"
#include "server/application/commandHandlers/createChannelCommandHandler/CreateChannelCommandHandler.h"
#include "server/application/commandHandlers/loginUserCommandHandler/LoginUserCommandHandler.h"
#include "server/application/commandHandlers/registerUserCommandHandler/RegisterUserCommandHandler.h"
#include "server/application/queryHandlers/findUsersChannelsByUserIdQueryHandler/FindUsersChannelsByUserIdQueryHandler.h"
#include "server/application/services/tokenService/TokenService.h"

namespace server::api
{
class MessageHandlerImpl : public MessageHandler
{
public:
    MessageHandlerImpl(std::shared_ptr<server::application::TokenService> tokenService,
                       std::unique_ptr<server::application::RegisterUserCommandHandler> registerUserCommandHandler,
                       std::unique_ptr<server::application::LoginUserCommandHandler> loginUserCommandHandler,
                       std::unique_ptr<server::application::CreateChannelCommandHandler> createChannelCommandHandler,
                       std::unique_ptr<server::application::FindUsersChannelsByUserIdQueryHandler>
                           findUsersChannelsByUserIdQueryHandler);

    common::messages::Message handleMessage(const common::messages::Message& message) override;

private:
    common::messages::Message handleRegisterMessage(const common::bytes::Bytes& payload) const;
    common::messages::Message handleLoginMessage(const common::bytes::Bytes& payload) const;
    common::messages::Message handleCreateChannelRequest(const common::bytes::Bytes& payload) const;
    common::messages::Message handleGetUserChannelsRequest(const common::bytes::Bytes& payload) const;

    std::shared_ptr<server::application::TokenService> tokenService;
    std::unique_ptr<server::application::RegisterUserCommandHandler> registerUserCommandHandler;
    std::unique_ptr<server::application::LoginUserCommandHandler> loginUserCommandHandler;
    std::unique_ptr<server::application::CreateChannelCommandHandler> createChannelCommandHandler;
    std::unique_ptr<server::application::FindUsersChannelsByUserIdQueryHandler> findUsersChannelsByUserIdQueryHandler;
};
}
