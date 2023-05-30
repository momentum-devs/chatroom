#pragma once

#include <gmock/gmock.h>

#include "MessageReader.h"

namespace common::messages
{
class MessageReaderMock : public MessageReader
{
    MOCK_METHOD(void, startReadingMessages, (std::function<void(const common::messages::Message&)>), ());
};
}
