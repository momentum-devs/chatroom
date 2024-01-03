#pragma once

#include <memory>

#include "MessageRouter.h"
#include "server/api/messageHandlers/MessageHandler.h"

namespace server::core
{
class MessageRouterImpl : public MessageRouter
{
public:
    MessageRouterImpl(
        std::unordered_map<common::messages::MessageId, std::shared_ptr<api::MessageHandler>> messageHandlers);

    common::messages::Message route(const common::messages::Message& message) override;

private:
    std::unordered_map<common::messages::MessageId, std::shared_ptr<api::MessageHandler>> messageHandlers;
};
}
