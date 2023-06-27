#include "UserChannel.h"

#include <utility>

namespace server::domain
{
UserChannel::UserChannel(std::string idInit, std::string userIdInit, std::string channelIdInit,
                         std::string createdAtInit, std::string updatedAtInit)
    : id{std::move(idInit)},
      userId{std::move(userIdInit)},
      channelId{std::move(channelIdInit)},
      createdAt{std::move(createdAtInit)},
      updatedAt{std::move(updatedAtInit)}
{
}

std::string UserChannel::getId() const
{
    return id;
}

std::string UserChannel::getUserId() const
{
    return userId;
}

std::string UserChannel::getChannelId() const
{
    return channelId;
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
