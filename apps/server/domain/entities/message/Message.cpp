#include "Message.h"

#include <utility>

namespace server::domain
{
Message::Message(std::string idInit, std::string contentInit, std::shared_ptr<User> senderInit,
                 std::shared_ptr<Channel> channelInit, std::shared_ptr<Group> groupInit, std::string createdAtInit,
                 std::string updatedAtInit)
    : id{std::move(idInit)},
      createdAt{std::move(createdAtInit)},
      updatedAt{std::move(updatedAtInit)},
      content{std::move(contentInit)},
      sender{std::move(senderInit)},
      channel{std::move(channelInit)},
      group{std::move(groupInit)}
{
}

std::string Message::getId() const
{
    return id;
}

std::string Message::getContent() const
{
    return content;
}

std::shared_ptr<User> Message::getSender() const
{
    return sender;
}

std::shared_ptr<Group> Message::getGroup() const
{
    return group;
}

std::shared_ptr<Channel> Message::getChannel() const
{
    return channel;
}

std::string Message::getCreatedAt() const
{
    return createdAt;
}

std::string Message::getUpdatedAt() const
{
    return updatedAt;
}

void Message::setContent(const std::string& newContent)
{
    content = newContent;
}

}
