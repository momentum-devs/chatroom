#pragma once

#include <gmock/gmock.h>

#include "MessageSender.h"

namespace common::messages
{
class MessageSenderMock : public MessageSender
{
    MOCK_METHOD(void, sendMessage, (const common::messages::Message&), ());
};
}
