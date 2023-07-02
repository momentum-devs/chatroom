#pragma once

#include <gmock/gmock.h>

#include "MessageFactory.h"

namespace client::api
{
class MessageFactoryMock : public MessageFactory
{
public:
    MOCK_METHOD(common::messages::Message, createMessage,
                (common::messages::MessageId messageId, const nlohmann::json& data,
                 const std::optional<std::string> token),
                (const override));
};
}
