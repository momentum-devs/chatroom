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
               std::string createdAtInit)
        : id{std::move(idInit)},
          created_at{std::move(createdAtInit)},
          user{std::move(userInit)},
          user_friend{std::move(userFriendInit)}
    {
    }

    [[nodiscard]] std::string getId() const
    {
        return id;
    }

    [[nodiscard]] std::shared_ptr<User> getUser() const
    {
        return user;
    }

    [[nodiscard]] std::shared_ptr<User> getUserFriend() const
    {
        return user_friend;
    }

    [[nodiscard]] std::string getCreatedAt() const
    {
        return created_at;
    }

private:
    Friendship() = default;

    friend class odb::access;

#pragma db id
    std::string id;
    std::string created_at;

#pragma db not_null
    std::shared_ptr<User> user;

#pragma db not_null
    std::shared_ptr<User> user_friend;
};
}
