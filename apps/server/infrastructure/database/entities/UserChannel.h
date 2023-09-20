#pragma once

#include <memory>
#include <odb/core.hxx>
#include <string>
#include <utility>

#include "Channel.h"
#include "User.h"

namespace server::infrastructure
{
#pragma db object table("users_channels")
class UserChannel
{
public:
    UserChannel(std::string idInit, std::shared_ptr<User> userInit, std::shared_ptr<Channel> channelInit,
                std::string createdAtInit, std::string updatedAtInit)
        : id{std::move(idInit)},
          created_at{std::move(createdAtInit)},
          updated_at{std::move(updatedAtInit)},
          user{std::move(userInit)},
          channel{std::move(channelInit)}
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

    std::shared_ptr<Channel> getChannel() const
    {
        return channel;
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
    UserChannel() = default;

    friend class odb::access;

#pragma db id
    std::string id;
    std::string created_at;
    std::string updated_at;

#pragma db not_null
    std::shared_ptr<User> user;

#pragma db not_null
    std::shared_ptr<Channel> channel;
};
}
