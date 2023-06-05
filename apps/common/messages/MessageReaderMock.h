#pragma once

#include <gmock/gmock.h>

#include "MessageReader.h"

namespace common::messages
{
class MessageReaderMock : public MessageReader
{
public:
    MOCK_METHOD(void, startReadingMessages, (std::function<void(const common::messages::Message&)>), (override));
    MOCK_METHOD(boost::asio::ip::tcp::socket&, getSocket, (), (override));
};
}
