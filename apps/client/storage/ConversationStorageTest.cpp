#include "ConversationStorage.h"

#include <gtest/gtest.h>

using namespace client::storage;
using namespace ::testing;

namespace
{

}

class ConversationStorageTest : public Test
{
public:
    ConversationStorage conversationStorage;
};

TEST_F(ConversationStorageTest, hasConversationReturnsFalseWhenConversationDoesNotExist)
{
    ASSERT_FALSE(conversationStorage.hasConversation("conversationId"));
}

TEST_F(ConversationStorageTest, hasConversationReturnsTrueWhenConversationExists)
{
    conversationStorage.createConversation("conversationId");

    ASSERT_TRUE(conversationStorage.hasConversation("conversationId"));
}

TEST_F(ConversationStorageTest, getExistingConversationReturnsConversation)
{
    auto conversation = conversationStorage.createConversation("conversationId");
    auto conversation2 = conversationStorage.createConversation("conversationId2");

    ASSERT_TRUE(conversationStorage.hasConversation("conversationId"));
    ASSERT_TRUE(conversationStorage.hasConversation("conversationId2"));
    ASSERT_EQ(conversation, conversationStorage.getConversation("conversationId"));
    ASSERT_EQ(conversation2, conversationStorage.getConversation("conversationId2"));
}
