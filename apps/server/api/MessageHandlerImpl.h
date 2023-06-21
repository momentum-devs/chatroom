#pragma once

#include <memory>
#include <optional>

#include "MessageHandler.h"
#include "server/application/commandHandlers/registerUserCommandHandler/RegisterUserCommandHandler.h"

namespace server::api
{
class MessageHandlerImpl : public MessageHandler
{
public:
    MessageHandlerImpl(std::unique_ptr<server::application::RegisterUserCommandHandler> registerUserCommandHandler);

    common::messages::Message handleMessage(const common::messages::Message& message) override;

private:
    common::messages::Message handleRegisterMessage(const common::bytes::Bytes& payload);

    static inline const common::bytes::Bytes nullToken =
        common::bytes::Bytes{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                             0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    std::optional<common::bytes::Bytes> token;
    std::unique_ptr<server::application::RegisterUserCommandHandler> registerUserCommandHandler;
};
}
