#include "Friendship.h"

#include <utility>

namespace server::domain
{
Friendship::Friendship(std::string idInit, std::shared_ptr<User> userInit, std::shared_ptr<User> userFriendInit,
                       std::string createdAtInit, std::string updatedAtInit)
    : id{std::move(idInit)},
      user{std::move(userInit)},
      userFriend{std::move(userFriendInit)},
      createdAt{std::move(createdAtInit)},
      updatedAt{std::move(updatedAtInit)}
{
}

std::string Friendship::getId() const
{
    return id;
}

std::shared_ptr<User> Friendship::getUser() const
{
    return user;
}

std::shared_ptr<User> Friendship::getUserFriend() const
{
    return userFriend;
}

std::string Friendship::getCreatedAt() const
{
    return createdAt;
}

std::string Friendship::getUpdatedAt() const
{
    return updatedAt;
}

}
