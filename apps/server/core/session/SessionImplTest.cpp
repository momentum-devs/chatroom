#include "SessionImpl.h"

#include <gtest/gtest.h>
#include <regex>

#include "MessageRouterMock.h"
#include "messages/MessageReaderMock.h"
#include "messages/MessageSenderMock.h"
#include "server/application/commandHandlers/user/logoutUserCommandHandler/LogoutUserCommandHandlerMock.h"
#include "server/application/services/tokenService/TokenServiceMock.h"

#include <fmt/format.h>

using namespace ::testing;
using namespace server::core;

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

    std::unique_ptr<MessageRouterMock> messageRouterMockInit = std::make_unique<StrictMock<MessageRouterMock>>();
    MessageRouterMock* messageHandlerMock = messageRouterMockInit.get();

    std::shared_ptr<server::application::TokenServiceMock> tokenServiceMock =
        std::make_shared<StrictMock<server::application::TokenServiceMock>>();

    std::unique_ptr<server::application::LogoutUserCommandHandlerMock> logoutUserCommandHandlerMockInit =
        std::make_unique<StrictMock<server::application::LogoutUserCommandHandlerMock>>();
    server::application::LogoutUserCommandHandlerMock* logoutUserCommandHandlerMock =
        logoutUserCommandHandlerMockInit.get();

    SessionImpl session{std::move(messageReaderMockInit), std::move(messageSenderMockInit),
                        std::move(messageRouterMockInit), tokenServiceMock,
                        std::move(logoutUserCommandHandlerMockInit)};

    std::function<void(const common::messages::Message&)> messageHandler;

    void startSession()
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
    startSession();

    EXPECT_CALL(*messageHandlerMock, route(message)).WillOnce(Return(responseMessage));
    EXPECT_CALL(*messageSenderMock, sendMessage(responseMessage));

    messageHandler(message);
}
