#include "Conversation.h"

#include <utility>

namespace server::domain
{
Conversation::Conversation(std::string idInit, std::shared_ptr<User> userInit, std::shared_ptr<User> recipientInit,
                           std::shared_ptr<Channel> channelInit, std::string createdAtInit, std::string updatedAtInit)
    : id{std::move(idInit)},
      createdAt{std::move(createdAtInit)},
      updatedAt{std::move(updatedAtInit)},
      user{std::move(userInit)},
      recipient{std::move(recipientInit)},
      channel{std::move(channelInit)}
{
}

std::string Conversation::getId() const
{
    return id;
}

std::shared_ptr<User> Conversation::getUser() const
{
    return user;
}

std::shared_ptr<User> Conversation::getRecipient() const
{
    return recipient;
}

std::shared_ptr<Channel> Conversation::getChannel() const
{
    return channel;
}

std::string Conversation::getCreatedAt() const
{
    return createdAt;
}

std::string Conversation::getUpdatedAt() const
{
    return updatedAt;
}

}
