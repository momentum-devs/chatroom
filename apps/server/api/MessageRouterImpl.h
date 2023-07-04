#pragma once

#include <memory>

#include "MessageHandler.h"
#include "MessageRouter.h"

namespace server::api
{
class MessageRouterImpl : public MessageRouter
{
public:
    MessageRouterImpl(std::unordered_map<common::messages::MessageId, std::shared_ptr<MessageHandler>> messageHandlers);
    common::messages::Message route(const common::messages::Message& message) override;

private:
    std::unordered_map<common::messages::MessageId, std::shared_ptr<MessageHandler>> messageHandlers;
};
}
