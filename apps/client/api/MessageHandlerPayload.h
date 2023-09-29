#pragma once

#include <ostream>
#include <tuple>

#include "common/messages/Message.h"

namespace client::api
{
struct MessageHandlerPayload
{
    common::messages::MessageId messageId;
    const std::string& name;
    std::function<void(const common::messages::Message&)> handler = nullptr;
};

inline bool operator==(const MessageHandlerPayload& lhs, const MessageHandlerPayload& rhs)
{
    auto tieStruct = [](const MessageHandlerPayload& messageHandlerPayload)
    { return std::tie(messageHandlerPayload.messageId, messageHandlerPayload.name); };

    return tieStruct(lhs) == tieStruct(rhs);
}
}