#include "SessionImpl.h"

#include <format>
#include <gtest/gtest.h>
#include <regex>

#include "MessageHandlerMock.h"
#include "messages/MessageReaderMock.h"
#include "messages/MessageSenderMock.h"

using namespace ::testing;
using namespace server::api;

namespace
{
const common::bytes::Bytes defaultPayload{};
const common::messages::Message message{common::messages::MessageId::Register, defaultPayload};
const common::messages::Message responseMessage{common::messages::MessageId::RegisterResponse, defaultPayload};
}

class SessionImplTest : public Test
{
public:
    std::unique_ptr<common::messages::MessageReaderMock> messageReaderMockInit =
        std::make_unique<StrictMock<common::messages::MessageReaderMock>>();
    common::messages::MessageReaderMock* messageReaderMock = messageReaderMockInit.get();

    std::unique_ptr<common::messages::MessageSenderMock> messageSenderMockInit =
        std::make_unique<StrictMock<common::messages::MessageSenderMock>>();
    common::messages::MessageSenderMock* messageSenderMock = messageSenderMockInit.get();

    std::unique_ptr<MessageHandlerMock> messageHandlerMockInit = std::make_unique<StrictMock<MessageHandlerMock>>();
    MessageHandlerMock* messageHandlerMock = messageHandlerMockInit.get();

    SessionImpl session{std::move(messageReaderMockInit), std::move(messageSenderMockInit),
                        std::move(messageHandlerMockInit)};

    std::function<void(const common::messages::Message&)> messageHandler;

    void startSesion()
    {
        EXPECT_CALL(*messageReaderMock, startReadingMessages(_)).WillOnce(SaveArg<0>(&messageHandler));

        session.startSession();
    }
};

TEST_F(SessionImplTest, startSession)
{
    EXPECT_CALL(*messageReaderMock, startReadingMessages(_));

    session.startSession();
}

TEST_F(SessionImplTest, sendMessage)
{
    EXPECT_CALL(*messageSenderMock, sendMessage(message));

    session.sendMessage(message);
}

TEST_F(SessionImplTest, handleReceivedMessage)
{
    startSesion();

    EXPECT_CALL(*messageHandlerMock, handleMessage(message)).WillOnce(Return(responseMessage));
    EXPECT_CALL(*messageSenderMock, sendMessage(responseMessage));

    messageHandler(message);
}
