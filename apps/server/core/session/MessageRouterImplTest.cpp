#include "MessageRouterImpl.h"

#include <gtest/gtest.h>

#include "server/api/messageHandlers/MessageHandlerMock.h"

using namespace ::testing;
using namespace server::core;

namespace
{
auto validMessage = common::messages::Message{common::messages::MessageId::Login, {}};
auto validMessageResponse = common::messages::Message{common::messages::MessageId::LoginResponse, {}};
auto invalidMessage = common::messages::Message{common::messages::MessageId::RegisterResponse, {}};
auto invalidMessageResponse = common::messages::Message{common::messages::MessageId::Error, {}};
}

class MessageRouterImplTest : public Test
{
public:
    std::shared_ptr<server::api::MessageHandlerMock> messageHandlerMock =
        std::make_shared<StrictMock<server::api::MessageHandlerMock>>();

    MessageRouterImpl messageRouter{{{common::messages::MessageId::Login, messageHandlerMock}}};
};

TEST_F(MessageRouterImplTest, handleValidMessage)
{
    EXPECT_CALL(*messageHandlerMock, handleMessage(validMessage)).WillOnce(Return(validMessageResponse));

    auto response = messageRouter.route(validMessage);

    EXPECT_EQ(response, validMessageResponse);
}

TEST_F(MessageRouterImplTest, handleInvalidMessage)
{
    auto response = messageRouter.route(invalidMessage);

    EXPECT_EQ(response, invalidMessageResponse);
}
