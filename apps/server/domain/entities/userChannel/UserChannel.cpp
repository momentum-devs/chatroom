#include "UserChannel.h"

#include <utility>

namespace server::domain
{
UserChannel::UserChannel(std::string idInit, std::shared_ptr<User> userInit, std::shared_ptr<Channel> channelInit,
                         std::string createdAtInit, std::string updatedAtInit)
    : id{std::move(idInit)},
      user{std::move(userInit)},
      channel{std::move(channelInit)},
      createdAt{std::move(createdAtInit)},
      updatedAt{std::move(updatedAtInit)}
{
}

std::string UserChannel::getId() const
{
    return id;
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

std::string UserChannel::getUpdatedAt() const
{
    return updatedAt;
}

}
