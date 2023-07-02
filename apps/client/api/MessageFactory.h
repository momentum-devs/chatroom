#pragma once

#include <nlohmann/json.hpp>

#include "messages/Message.h"

namespace client::api
{
class MessageFactory
{
public:
    virtual ~MessageFactory() = default;

    virtual common::messages::Message createMessage(common::messages::MessageId messageId, const nlohmann::json& data,
                                                    const std::optional<std::string> token) const = 0;
};
}