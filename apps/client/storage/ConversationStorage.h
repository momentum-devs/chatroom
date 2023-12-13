#pragma once

#include <memory>
#include <unordered_map>

#include "MessageStorage.h"

namespace client::storage
{
class ConversationStorage
{
public:
    bool hasConversation(const std::string& conversationId) const;
    std::shared_ptr<MessageStorage> getConversation(const std::string& conversationId) const;
    std::shared_ptr<MessageStorage> createConversation(const std::string& conversationId);

private:
    std::unordered_map<std::string, std::shared_ptr<MessageStorage>> conversations;
};
}