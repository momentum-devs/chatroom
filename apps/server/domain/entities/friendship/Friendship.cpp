#include "Friendship.h"

#include <utility>

namespace server::domain
{
Friendship::Friendship(std::string idInit, std::shared_ptr<User> userInit, std::shared_ptr<User> userFriendInit,
                       std::string createdAtInit, std::shared_ptr<Group> groupInit)
    : id{std::move(idInit)},
      createdAt{std::move(createdAtInit)},
      user{std::move(userInit)},
      userFriend{std::move(userFriendInit)},
      group{std::move(groupInit)}
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

std::shared_ptr<Group> Friendship::getGroup() const
{
    return group;
}

}
