#pragma once

#include <memory>
#include <odb/core.hxx>
#include <string>
#include <utility>

#include "User.h"

namespace server::infrastructure
{
#pragma db object table("friendships")
class Friendship
{
public:
    Friendship(std::string idInit, std::shared_ptr<User> userInit, std::shared_ptr<User> userFriendInit,
               std::string createdAtInit, std::string updatedAtInit)
        : id{std::move(idInit)},
          user{std::move(userInit)},
          user_friend{std::move(userFriendInit)},
          created_at{std::move(createdAtInit)},
          updated_at{std::move(updatedAtInit)}
    {
    }

    std::string getId() const
    {
        return id;
    }

    std::shared_ptr<User> getUser() const
    {
        return user;
    }

    std::shared_ptr<User> getUserFriend() const
    {
        return user_friend;
    }

    std::string getCreatedAt() const
    {
        return created_at;
    }

    std::string getUpdatedAt() const
    {
        return updated_at;
    }

private:
    Friendship() = default;

    friend class odb::access;

#pragma db id
    std::string id;
    std::string created_at;
    std::string updated_at;

#pragma db not_null
    std::shared_ptr<User> user;

#pragma db not_null
    std::shared_ptr<User> user_friend;
};
}
