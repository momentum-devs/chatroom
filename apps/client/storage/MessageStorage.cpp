#include "MessageStorage.h"

namespace client::storage
{
void MessageStorage::addMessage(std::shared_ptr<types::Message> message)
{
    if (messagesIdMap.contains(message->messageId.toStdString()))
    {
        return;
    }

    if (not messages.empty() and message->sendTime < messages.back()->sendTime)
    {
        if (message->sendTime < messages.front()->sendTime)
        {
            addMessageBefore(message, messages.begin());
            return;
        }

        for (auto it = messages.rbegin(); it != messages.rend(); ++it)
        {
            if (message->sendTime >= (*it)->sendTime)
            {
                addMessageBefore(message, std::next(it).base());
                return;
            }
        }
    }

    addMessageBefore(message, messages.end());
}

void MessageStorage::addMessageBefore(std::shared_ptr<types::Message> message,
                                      std::list<std::shared_ptr<types::Message>>::iterator it)
{
    if (not messages.empty() and it != messages.begin())
    {
        message->showSeparator = std::prev(it)->get()->sendTime < message->sendTime.addDays(-1);
        message->showNameAndDate = std::prev(it)->get()->senderName != message->senderName or
                                   std::prev(it)->get()->sendTime.addSecs(5 * 60) < message->sendTime;
    }
    else
    {
        message->showSeparator = false;
        message->showNameAndDate = true;
    }

    if (it != messages.end())
    {
        it->get()->showSeparator = message->sendTime < it->get()->sendTime.addDays(-1);
        it->get()->showNameAndDate =
            it->get()->senderName != message->senderName or message->sendTime.addSecs(5 * 60) < it->get()->sendTime;
    }

    messages.insert(it, message);
    messagesIdMap[message->messageId.toStdString()] = message;
}

void MessageStorage::clearMessages()
{
    messages.clear();
    messagesIdMap.clear();
}

const std::list<std::shared_ptr<types::Message>>& MessageStorage::getMessages() const
{
    return messages;
}

std::shared_ptr<types::Message> MessageStorage::getMessage(const std::string& messageId) const
{
    return messagesIdMap.at(messageId);
}

std::shared_ptr<types::Message> MessageStorage::getLatestMessage() const
{
    return messages.back();
}
}