#include "Message.h"

#include <gtest/gtest.h>

#include "faker-cxx/Internet.h"
#include "faker-cxx/Lorem.h"
#include "faker-cxx/String.h"

using namespace client::types;

class MessageTest : public ::testing::Test
{
};

TEST_F(MessageTest, shouldCreateMessage)
{
    const QString messageText = faker::Lorem::sentence().c_str();
    const QString senderName = faker::Internet::username().c_str();
    const QString messageId = faker::String::uuid().c_str();
    const QDateTime sendTime = QDateTime::currentDateTime();

    const Message message{messageText, senderName, messageId, sendTime};

    ASSERT_EQ(messageText, message.property("messageText").toString());
    ASSERT_EQ(senderName, message.property("senderName").toString());
    ASSERT_EQ(messageId, message.property("messageId").toString());
    ASSERT_EQ(sendTime, message.property("sendTime").toDateTime());
}

TEST_F(MessageTest, shouldReturnTrueWhenMessagesAreEqual)
{
    const QString messageText = faker::Lorem::sentence().c_str();
    const QString senderName = faker::Internet::username().c_str();
    const QString messageId = faker::String::uuid().c_str();
    const QDateTime sendTime = QDateTime::currentDateTime();

    const Message message{messageText, senderName, messageId, sendTime};

    const Message messageCopy{message};

    ASSERT_EQ(message, messageCopy);
}