#include "ConversationStorage.h"

namespace client::storage
{
bool ConversationStorage::hasConversation(const std::string& conversationId) const
{
    return conversations.contains(conversationId);
}

std::shared_ptr<MessageStorage> ConversationStorage::getConversation(const std::string& conversationId) const
{
    return conversations.at(conversationId);
}

std::shared_ptr<MessageStorage> ConversationStorage::createConversation(const std::string& conversationId)
{
    auto messageStorage = std::make_shared<MessageStorage>();
    conversations[conversationId] = messageStorage;
    return messageStorage;
}
}