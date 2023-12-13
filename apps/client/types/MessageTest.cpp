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
    const auto fromUser = true;
    const QString messageText = faker::Lorem::sentence().c_str();
    const QString senderName = faker::Internet::username().c_str();
    const QString messageId = faker::String::uuid().c_str();
    const QDateTime sendTime = QDateTime::currentDateTime();

    const Message message{fromUser, messageText, senderName, messageId, sendTime, nullptr};

    ASSERT_EQ(fromUser, message.property("fromUser").toBool());
    ASSERT_EQ(messageText, message.property("messageText").toString());
    ASSERT_EQ(senderName, message.property("senderName").toString());
    ASSERT_EQ(messageId, message.property("messageId").toString());
    ASSERT_EQ(sendTime, message.property("sendTime").toDateTime());
}

TEST_F(MessageTest, shouldCreateMessageWithPreviousMessage)
{
    const auto fromUser = true;
    const QString messageText = faker::Lorem::sentence().c_str();
    const QString senderName = faker::Internet::username().c_str();
    const QString messageId = faker::String::uuid().c_str();
    const QDateTime sendTime = QDateTime::currentDateTime();

    const Message previousMessage{fromUser, messageText, senderName, messageId, sendTime, nullptr};

    const auto newFromUser = false;
    const QString newMessageText = faker::Lorem::sentence().c_str();
    const QString newSenderName = faker::Internet::username().c_str();
    const QString newMessageId = faker::String::uuid().c_str();
    const QDateTime newSendTime = QDateTime::currentDateTime();

    const Message message{newFromUser,  newMessageText, newSenderName,
                          newMessageId, newSendTime,    std::make_shared<Message>(previousMessage)};

    ASSERT_EQ(newFromUser, message.property("fromUser").toBool());
    ASSERT_EQ(newMessageText, message.property("messageText").toString());
    ASSERT_EQ(newSenderName, message.property("senderName").toString());
    ASSERT_EQ(newMessageId, message.property("messageId").toString());
    ASSERT_EQ(newSendTime, message.property("sendTime").toDateTime());
}

TEST_F(MessageTest, shouldReturnTrueWhenMessagesAreEqual)
{
    const auto fromUser = true;
    const QString messageText = faker::Lorem::sentence().c_str();
    const QString senderName = faker::Internet::username().c_str();
    const QString messageId = faker::String::uuid().c_str();
    const QDateTime sendTime = QDateTime::currentDateTime();

    const Message message{fromUser, messageText, senderName, messageId, sendTime, nullptr};

    const Message messageCopy{message};

    ASSERT_EQ(message, messageCopy);
}

TEST_F(MessageTest, shouldReturnTrueWhenSeparatorsShouldBeShown)
{
    const auto fromUser = true;
    const QString messageText = faker::Lorem::sentence().c_str();
    const QString senderName = faker::Internet::username().c_str();
    const QString messageId = faker::String::uuid().c_str();
    const QDateTime sendTime = QDateTime::currentDateTime();

    const Message previousMessage{fromUser, messageText, senderName, messageId, sendTime, nullptr};

    const auto newFromUser = false;
    const QString newMessageText = faker::Lorem::sentence().c_str();
    const QString newSenderName = faker::Internet::username().c_str();
    const QString newMessageId = faker::String::uuid().c_str();
    const QDateTime newSendTime = QDateTime::currentDateTime().addDays(1);

    const Message message{newFromUser,  newMessageText, newSenderName,
                          newMessageId, newSendTime,    std::make_shared<Message>(previousMessage)};

    ASSERT_TRUE(message.property("showSeparator").toBool());
}