#include "MessageStorage.h"

#include <gtest/gtest.h>

using namespace client;
using namespace client::storage;
using namespace ::testing;

namespace
{

}

class MessageStorageTest : public Test
{
public:
    MessageStorage messageStorage;
};

TEST_F(MessageStorageTest, addMessage)
{
    auto message =
        std::make_shared<types::Message>("messageText", "senderName", "messageId", QDateTime::currentDateTime());

    messageStorage.addMessage(message);

    ASSERT_EQ(messageStorage.getMessages().size(), 1);
    ASSERT_EQ(messageStorage.getMessages().back(), message);
    ASSERT_EQ(messageStorage.getMessage("messageId"), message);
    ASSERT_EQ(messageStorage.getLatestMessage(), message);
    ASSERT_TRUE(messageStorage.hasMessage("messageId"));
}

TEST_F(MessageStorageTest, clearMessages)
{
    auto message =
        std::make_shared<types::Message>("messageText", "senderName", "messageId", QDateTime::currentDateTime());

    messageStorage.addMessage(message);

    messageStorage.clearMessages();

    ASSERT_EQ(messageStorage.getMessages().size(), 0);
    ASSERT_FALSE(messageStorage.hasMessage("messageId"));
}

TEST_F(MessageStorageTest, getLastMessage)
{
    auto message =
        std::make_shared<types::Message>("messageText", "senderName", "messageId", QDateTime::currentDateTime());
    auto message2 =
        std::make_shared<types::Message>("messageText2", "senderName2", "messageId2", QDateTime::currentDateTime());

    messageStorage.addMessage(message);
    messageStorage.addMessage(message2);

    ASSERT_EQ(messageStorage.getLatestMessage(), message2);
    ASSERT_TRUE(messageStorage.hasMessage("messageId"));
    ASSERT_TRUE(messageStorage.hasMessage("messageId2"));
}

TEST_F(MessageStorageTest, addExistingMessage)
{
    auto message =
        std::make_shared<types::Message>("messageText", "senderName", "messageId", QDateTime::currentDateTime());

    messageStorage.addMessage(message);
    messageStorage.addMessage(message);

    ASSERT_EQ(messageStorage.getMessages().size(), 1);
    ASSERT_TRUE(messageStorage.hasMessage("messageId"));
}

TEST_F(MessageStorageTest, addMessageWithSendTimeLessThanLatestMessage)
{
    auto message =
        std::make_shared<types::Message>("messageText", "senderName", "messageId", QDateTime::currentDateTime());
    auto message2 = std::make_shared<types::Message>("messageText2", "senderName2", "messageId2",
                                                     QDateTime::currentDateTime().addSecs(-1));

    messageStorage.addMessage(message);
    messageStorage.addMessage(message2);

    ASSERT_EQ(messageStorage.getLatestMessage(), message);

    ASSERT_TRUE(messageStorage.hasMessage("messageId"));
    ASSERT_TRUE(messageStorage.hasMessage("messageId2"));
}

TEST_F(MessageStorageTest, addMessageWithSendTimeOver5MinutesLessThanLatestMessage)
{
    auto message =
        std::make_shared<types::Message>("messageText", "senderName", "messageId", QDateTime::currentDateTime());
    auto message2 = std::make_shared<types::Message>("messageText2", "senderName2", "messageId2",
                                                     QDateTime::currentDateTime().addSecs(-5 * 60).addSecs(-1));

    messageStorage.addMessage(message);
    messageStorage.addMessage(message2);

    ASSERT_EQ(messageStorage.getLatestMessage()->messageId, message->messageId);
    ASSERT_EQ(messageStorage.getLatestMessage()->showNameAndDate, true);
    ASSERT_EQ(messageStorage.getLatestMessage()->showSeparator, false);
    ASSERT_TRUE(messageStorage.hasMessage("messageId"));
    ASSERT_TRUE(messageStorage.hasMessage("messageId2"));
}

TEST_F(MessageStorageTest, addMessageWithSendTimeOverDayLessThanLatestMessage)
{
    auto message =
        std::make_shared<types::Message>("messageText", "senderName", "messageId", QDateTime::currentDateTime());
    auto message2 = std::make_shared<types::Message>("messageText2", "senderName2", "messageId2",
                                                     QDateTime::currentDateTime().addDays(-1).addSecs(-1));

    messageStorage.addMessage(message);
    messageStorage.addMessage(message2);

    ASSERT_EQ(messageStorage.getLatestMessage()->messageId, message->messageId);
    ASSERT_EQ(messageStorage.getLatestMessage()->showNameAndDate, true);
    ASSERT_EQ(messageStorage.getLatestMessage()->showSeparator, true);
    ASSERT_TRUE(messageStorage.hasMessage("messageId"));
    ASSERT_TRUE(messageStorage.hasMessage("messageId2"));
}

TEST_F(MessageStorageTest, addMessageWithSendTimeOver5MinutesGreaterThanLatestMessage)
{
    auto message =
        std::make_shared<types::Message>("messageText", "senderName", "messageId", QDateTime::currentDateTime());
    auto message2 = std::make_shared<types::Message>("messageText2", "senderName2", "messageId2",
                                                     QDateTime::currentDateTime().addSecs(5 * 60).addSecs(1));

    messageStorage.addMessage(message);
    messageStorage.addMessage(message2);

    ASSERT_EQ(messageStorage.getLatestMessage()->messageId, message2->messageId);
    ASSERT_EQ(messageStorage.getLatestMessage()->showNameAndDate, true);
    ASSERT_EQ(messageStorage.getLatestMessage()->showSeparator, false);
    ASSERT_TRUE(messageStorage.hasMessage("messageId"));
    ASSERT_TRUE(messageStorage.hasMessage("messageId2"));
}

TEST_F(MessageStorageTest, addMessageWithSendTimeOverDayGreaterThanLatestMessage)
{
    auto message =
        std::make_shared<types::Message>("messageText", "senderName", "messageId", QDateTime::currentDateTime());
    auto message2 = std::make_shared<types::Message>("messageText2", "senderName2", "messageId2",
                                                     QDateTime::currentDateTime().addDays(1).addSecs(1));

    messageStorage.addMessage(message);
    messageStorage.addMessage(message2);

    ASSERT_EQ(messageStorage.getLatestMessage()->messageId, message2->messageId);
    ASSERT_EQ(messageStorage.getLatestMessage()->showNameAndDate, true);
    ASSERT_EQ(messageStorage.getLatestMessage()->showSeparator, true);
    ASSERT_TRUE(messageStorage.hasMessage("messageId"));
    ASSERT_TRUE(messageStorage.hasMessage("messageId2"));
}