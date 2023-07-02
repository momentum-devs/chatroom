#include "SessionImpl.h"

#include <boost/beast/_experimental/test/stream.hpp>
#include <gtest/gtest.h>

#include "MessageFactoryMock.h"
#include "messages/MessageReaderMock.h"
#include "messages/MessageSenderMock.h"
#include "SocketConnectorMock.h"

#include "RemoveHandlerError.h"

using namespace ::testing;
using namespace client::api;

namespace
{
struct DummyMock
{
    MOCK_METHOD(void, foo, (), ());
};

const ConnectorPayload connectorPayload{"host", 123};
const common::messages::MessageId messageId{common::messages::MessageId::Register};
const common::messages::Message message{messageId, {}};
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

    std::unique_ptr<SocketConnectorMock> socketConnectorMockInit = std::make_unique<StrictMock<SocketConnectorMock>>();
    SocketConnectorMock* socketConnectorMock = socketConnectorMockInit.get();

    std::unique_ptr<MessageFactoryMock> messageFactoryMockInit = std::make_unique<StrictMock<MessageFactoryMock>>();
    MessageFactoryMock* messageFactoryMock = messageFactoryMockInit.get();

    SessionImpl session{std::move(messageReaderMockInit), std::move(messageSenderMockInit),
                        std::move(socketConnectorMockInit), std::move(messageFactoryMockInit)};

    std::function<void(const common::messages::Message&)> messageHandler;

    void connectSession()
    {
        EXPECT_CALL(*socketConnectorMock, connect(connectorPayload));
        EXPECT_CALL(*messageReaderMock, startReadingMessages(_)).WillOnce(SaveArg<0>(&messageHandler));

        session.connect(connectorPayload);
    }

    std::shared_ptr<DummyMock> dummyMock = std::make_shared<StrictMock<DummyMock>>();
};

TEST_F(SessionImplTest, connectSession)
{
    connectSession();
}

TEST_F(SessionImplTest, sendMessage)
{
    EXPECT_CALL(*messageSenderMock, sendMessage(message));

    session.sendMessage(message);
}

TEST_F(SessionImplTest, sessionGetMessage_shouldCallRegisteredHandler)
{
    connectSession();
    EXPECT_CALL(*dummyMock, foo());

    session.addMessageHandler(
        {messageId, "handlerName", [this](const common::messages::Message&) { dummyMock->foo(); }});

    messageHandler(message);
}

TEST_F(SessionImplTest, sessionGetMessage_shouldNotCallRemovedHandler)
{
    connectSession();

    session.addMessageHandler({messageId, "notCall", [](const common::messages::Message&) { FAIL(); }});
    session.removeMessageHandler({messageId, "notCall", {}});

    messageHandler(message);
}

TEST_F(SessionImplTest, removeNonExistingHandler_shouldThrow)
{
    session.addMessageHandler({messageId, "notCall", [](const common::messages::Message&) { FAIL(); }});
    EXPECT_THROW(session.removeMessageHandler({messageId, "notExisting", {}}), RemoveHandlerError);
}

TEST_F(SessionImplTest, removeNonExistingMessageIdHandler_shouldThrow)
{
    EXPECT_THROW(session.removeMessageHandler({messageId, "notExisting", {}}), RemoveHandlerError);
}