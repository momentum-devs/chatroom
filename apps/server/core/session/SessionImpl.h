#pragma once

#include "common/messages/MessageReader.h"
#include "common/messages/MessageSender.h"
#include "MessageRouter.h"
#include "server/application/commandHandlers/user/logoutUserCommandHandler/LogoutUserCommandHandler.h"
#include "server/application/services/tokenService/TokenService.h"
#include "Session.h"

namespace server::core
{
class SessionImpl final : public Session
{
public:
    SessionImpl(std::unique_ptr<common::messages::MessageReader> messageReader,
                std::unique_ptr<common::messages::MessageSender> messageSender,
                std::unique_ptr<MessageRouter> messageRouter,
                std::shared_ptr<server::application::TokenService> tokenService,
                std::unique_ptr<server::application::LogoutUserCommandHandler> logoutUserCommandHandler);

    void startSession() override;
    void sendMessage(const common::messages::Message& message) override;
    bool isActive() override;
    void close() override;

private:
    void startReceivingMessage();
    void handleMessage(const common::messages::Message& message);
    void storeCurrentUser(const common::messages::Message& message);

    std::optional<std::string> userId;

    std::unique_ptr<common::messages::MessageReader> messageReader;
    std::unique_ptr<common::messages::MessageSender> messageSender;
    std::unique_ptr<MessageRouter> messageRouter;
    std::shared_ptr<server::application::TokenService> tokenService;
    std::unique_ptr<server::application::LogoutUserCommandHandler> logoutUserCommandHandler;
};
}
