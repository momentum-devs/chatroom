#pragma once

#include "MessageFactory.h"

namespace client::api
{
class MessageFactoryImpl : public MessageFactory
{
public:
    common::messages::Message createMessage(common::messages::MessageId messageId, const nlohmann::json& data,
                                            const std::optional<std::string> token) const override;
};
}
