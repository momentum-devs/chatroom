#include "MessageFactoryImpl.h"

#include <gtest/gtest.h>

#include "fmt/format.h"

using namespace ::testing;
using namespace client::api;

namespace
{
common::messages::MessageId messageId = common::messages::MessageId::Error;
nlohmann::json data{};
std::string token = "token";
std::string payloadWithoutToken{fmt::format(R"({{"data":{}}})", data.dump())};
common::messages::Message messageWithoutToken{messageId, common::bytes::Bytes(payloadWithoutToken)};
std::string payloadWithToken{fmt::format(R"({{"data":{},"token":"{}"}})", data.dump(), token)};
common::messages::Message messageWithToken{messageId, common::bytes::Bytes(payloadWithToken)};
}

class MessageFactoryImplTest : public Test
{
public:
    MessageFactoryImpl messageFactory{};
};

TEST_F(MessageFactoryImplTest, createMessageWithoutToken)
{
    auto message = messageFactory.createMessage(messageId, data, std::nullopt);

    EXPECT_EQ(message, messageWithoutToken);
}

TEST_F(MessageFactoryImplTest, createMessageWithToken)
{
    auto message = messageFactory.createMessage(messageId, data, token);

    EXPECT_EQ(message, messageWithToken);
}
