#include "MessageRouterImpl.h"

namespace server::core
{
MessageRouterImpl::MessageRouterImpl(
    std::unordered_map<common::messages::MessageId, std::shared_ptr<api::MessageHandler>> messageHandlersInt)
    : messageHandlers{std::move(messageHandlersInt)}
{
}

common::messages::Message MessageRouterImpl::route(const common::messages::Message& message)
{
    if (messageHandlers.contains(message.id))
    {
        return messageHandlers.at(message.id)->handleMessage(message);
    }

    return {common::messages::MessageId::Error, {}};
}
}
