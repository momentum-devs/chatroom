#include "UserChannel.h"

#include <utility>

namespace server::domain
{
UserChannel::UserChannel(std::string idInit, std::optional<std::string> lastReadMessageIdInit,
                         std::shared_ptr<User> userInit, std::shared_ptr<Channel> channelInit,
                         std::string createdAtInit)
    : id{std::move(idInit)},
      createdAt{std::move(createdAtInit)},
      lastReadMessageId{std::move(lastReadMessageIdInit)},
      user{std::move(userInit)},
      channel{std::move(channelInit)}
{
}

std::string UserChannel::getId() const
{
    return id;
}

std::optional<std::string> UserChannel::getLastReadMessageId() const
{
    return lastReadMessageId;
}

std::shared_ptr<User> UserChannel::getUser() const
{
    return user;
}

std::shared_ptr<Channel> UserChannel::getChannel() const
{
    return channel;
}

std::string UserChannel::getCreatedAt() const
{
    return createdAt;
}

}
