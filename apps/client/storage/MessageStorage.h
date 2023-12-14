#pragma once

#include <list>
#include <memory>
#include <unordered_map>

#include "client/types/Message.h"

namespace client::storage
{
class MessageStorage
{
public:
    void addMessage(std::shared_ptr<types::Message> message);
    void clearMessages();
    const std::list<std::shared_ptr<types::Message>>& getMessages() const;
    std::shared_ptr<types::Message> getMessage(const std::string& messageId) const;
    std::shared_ptr<types::Message> getLatestMessage() const;
    bool hasMessage(const std::string& messageId) const;

private:
    void addMessageBefore(std::shared_ptr<types::Message> message,
                          std::list<std::shared_ptr<types::Message>>::iterator it);
    std::list<std::shared_ptr<types::Message>> messages;
    std::unordered_map<std::string, std::shared_ptr<types::Message>> messagesIdMap;
};
}